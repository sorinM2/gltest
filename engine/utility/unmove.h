#pragma once

namespace util
{
	
	template<class T>
	T& unmove(T&& t)
	{
	    return t;
	}
}
