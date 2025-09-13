#pragma once 

namespace utl 
{

constexpr bool is_same_as(float a, float b)
{
	float delta = 0.001;
	float dif =  a - b;
	if ( dif < delta and dif > -delta )
		return true;
	return false;
}

constexpr void keep_aspect_ratio(int& width, int& height, float _aspect_ratio)
{
	float new_aspect = ( float(width) / float(height) );

	if ( is_same_as(new_aspect, _aspect_ratio))
		return;

	if ( new_aspect > _aspect_ratio ) 
		width = height * _aspect_ratio;
	else height = width / _aspect_ratio;
}

}
