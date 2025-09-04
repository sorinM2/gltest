#pragma once

namespace utl
{
	
	template<class T>
	T& unmove(T&& t)
	{
	    return t;
	}
}
