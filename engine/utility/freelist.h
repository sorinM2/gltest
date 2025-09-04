#pragma once
#include <vector>
#include <queue>
#include <new>

namespace util
{

template <typename T>
class freelist 
{
public:
	template<typename... Targs>
	unsigned int emplace_back(Targs&&... args)
	{
		if ( _free_slots.empty() )
		{
			_data.emplace_back(std::forward<Targs>(args)...);
			return _data.size();
		} else {
			unsigned int index = _free_slots.front();
			_free_slots.pop();
			new (&_data[index]) T(std::forward<Targs>(args)...);
			return index;
		}
	}

private:
	std::vector<T> _data;
	std::queue<unsigned int> _free_slots; 
};

}
