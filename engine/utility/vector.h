#pragma once
#include <cassert>
#include <new>
#include <utility>
#include <algorithm>
#include <cstring>

namespace utl
{

template<typename T, bool disable_tombstoning = true>
class vector
{
public:
	static_assert(disable_tombstoning or sizeof(T) >= sizeof(unsigned int));

	vector()
	{
		reserve(1);
	}

	vector(vector& other)
	{
		*this = other;
	}

	vector(const vector& other)
	{
		*this = other;
	}

	constexpr vector& operator=(const vector& other)
	{
		if ( this == &other )
			return *this;
		
		clear();
		reserve(other.size());
		
		for ( auto& c : other)
			if constexpr (disable_tombstoning)
				emplace_back(c);
			else if ( !other.is_tombstone(&c) )
					emplace_back(c);
			else {
				memset(end(), 'd', sizeof(T));
				_size++;
			}
		
		return *this;
	}

	vector(vector&& other )
	{
		*this = std::move(other);
	}

	constexpr vector& operator=(vector<T, disable_tombstoning>&& other)
	{
		clear();
		::operator delete(_data);

		_data = other._data;
		_size = other._size;
		_capacity = other._capacity;

		other._data = nullptr;
		other._size = other._capacity = 0;

		return *this;
	}

	vector(unsigned int size)
	{
		reserve(std::max(size, 1u));
		for ( unsigned int i = 0; i < size; ++i )
			emplace_back();
	}

	vector(unsigned int size, const T& value)
	{
		assert(size);
		resize(size, value);
	}

	constexpr void push_back(const T& value)
	{
		emplace_back(value);
	}

	constexpr void push_back(T&& value)
	{
		emplace_back(std::move(value));
	}

	template<typename... Targs>
	constexpr void emplace_back(Targs&&... args)
	{
		controlled_reserve(_size + 1);
		new (_data + _size) T(std::forward<Targs>(args)...);
		++ _size;
	}

	constexpr T* begin() const
	{
		return _data;
	}

	constexpr T* end() const
	{
		return _data + _size;
	}

	constexpr void insert(T* position, const T& value)
	{
		emplace(position, value);
	}

	constexpr void insert(T* position, T* range_start, T* range_end)
	{
	    static_assert(disable_tombstoning);
		assert(range_end > range_start);
		assert(position >= begin() && position <= end());

		unsigned int spaces = range_end - range_start;

		unsigned int offset = position - begin();

		controlled_reserve(_size + spaces);

		position = begin() + offset;

		for ( T* it = end() + spaces - 1; it >= position + spaces; --it )
		{
			new (it) T(std::move(*(it - spaces)));
			(it - spaces)->~T();
		}

		for ( unsigned int inc = 0; inc < spaces; ++inc )
			new (position + inc) T(*(range_start + inc));

		_size += spaces;
	}


	template<typename... Targs>
	constexpr void emplace(T* position, Targs&&... args)
	{

		assert(position >= begin() && position <= end() );

		unsigned int offset = position - begin();

		controlled_reserve(_size + 1);

		position = begin() + offset;

		for ( T* it = end(); it > position; --it )
		{
			if constexpr (disable_tombstoning)
			{
				new (it) T(std::move(*(it - 1)));
				(it - 1)->~T();
			}
			else
			{
				if ( !is_tombstone(it - 1) )
				{
					new (it) T(std::move(*(it - 1)));
					(it - 1)->~T();
				}
				else
					memset(it, 'd', sizeof(T));
			}
		}
		new (position) T(std::forward<Targs>(args)...);

		++ _size;
	}

	constexpr void erase(T* position)
	{
		erase(position, position + 1);
	}

	constexpr void erase(T* range_start, T* range_end )
	{
		assert(_data);
		assert(range_start <= range_end);
		assert(begin() <= range_start && range_end <= end());

		unsigned int spaces = range_end - range_start;
		unsigned int spaces_to_move = end() - range_end;

		for ( T* it = range_start; it < range_end; ++it )
            if constexpr (disable_tombstoning)
			    it->~T();
            else if ( !is_tombstone(it) )
		    {
			    it->~T();
			    mark_as_tombstone(it);
		    }
        if constexpr (disable_tombstoning){
		    for ( unsigned int inc = 0; inc < spaces_to_move; ++inc )
			    new (range_start + inc) T(std::move(*(range_end + inc)));
		    _size -= spaces;
        }
	}

	constexpr void controlled_reserve(unsigned int needed_capacity)
	{
		if ( needed_capacity > _capacity )
			reserve(std::max(needed_capacity, _capacity * 3 / 2 + 1));
	}

	constexpr void reserve(unsigned int new_capacity)
	{
		if ( new_capacity > _capacity )
		{
			T* new_buffer = reinterpret_cast<T*>(::operator new(new_capacity * sizeof(T)));
			for ( unsigned int inc = 0; inc < _size; ++inc )
			{
				if constexpr (disable_tombstoning)
				{
				    new (new_buffer + inc) T(std::move(*(_data + inc)));
				    (_data + inc)->~T();
				}
				else if ( !is_tombstone(_data + inc) )
				{
					new (new_buffer + inc) T(std::move(*(_data + inc)));
					(_data + inc)->~T();
				} 
				else memset(new_buffer + inc, 'd', sizeof(T));
			}
			::operator delete(_data);
			_data = new_buffer;
			_capacity = new_capacity;
		}
	}

	constexpr void resize(unsigned int size)
	{
		controlled_reserve(std::max(size, 1u));
		while ( _size < size )
			emplace_back();
	}

	constexpr void resize(unsigned int size, const T& value)
	{
		controlled_reserve(size);
		while ( _size < size )
			emplace_back(value);
	}

	constexpr unsigned int size() const
	{
		return _size;
	}

	constexpr bool empty() const
	{
		return _size == 0;
	}

	constexpr unsigned int capacity() const
	{
		return _capacity;
	}

	constexpr T* data() const
	{
		return _data;
	}
	constexpr void clear()
	{
		if ( !_capacity )
			return;

		for ( auto& c : *this )
		{
            if constexpr (disable_tombstoning)
			    c.~T();
            else if ( !is_tombstone(&c) )
				c.~T();
		}
		
		::operator delete(_data);
		_data = nullptr;
		_size = _capacity = 0;

		reserve(1);
	}



	constexpr T& operator[]( unsigned int pos)
	{
		assert(pos >= 0 && pos < _size );
		if ( !disable_tombstoning )
			assert(!is_tombstone(_data + pos));
		return *(_data + pos);
	}

	constexpr const T& operator[](unsigned int pos) const
	{
		assert(pos >= 0 && pos < _size );
		if ( !disable_tombstoning )
			assert(!is_tombstone(_data + pos));
		return *(_data + pos);
	}

	~vector()
	{
		for ( T* value = begin(); value < end(); ++value )
            if constexpr (disable_tombstoning)
			    value->~T();
            else{
			    if ( !is_tombstone(value) )
				    value->~T();
            }

		::operator delete(_data);
	}

	constexpr bool is_tombstone(T* position) const
	{
        static_assert(!disable_tombstoning);
		assert(position >= _data && position < _data + _size);
		unsigned char* it = reinterpret_cast<unsigned char*>(position);
		for ( unsigned int inc = 0; inc < sizeof(T); ++ inc )
			if ( *(it + inc) != 'd' )
				return false;
		return true;
	}

	constexpr void mark_as_tombstone(T* position)
	{
	    static_assert(!disable_tombstoning);
		assert(position >= _data && position < _data + _size);
		memset(position, 'd', sizeof(T));
	}
private:
	T* _data = nullptr;
	unsigned int _size = 0;
	unsigned int _capacity = 0;
};

}
