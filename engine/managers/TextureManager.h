#pragma once
#include "core/GLCommon.h"
#include "utility/vector.h"
#include <string>
namespace textures 
{

class texture_2d
{
public:
	void initialize(std::string path, GLenum internal_format, GLenum format);
	void bind(unsigned int slot);
	void unbind();

private:
	bool binded = false;
	unsigned int _slot;

	unsigned int _id;
	std::string _path = "";
};


void add_texture(std::string path, GLenum internal_format, GLenum format);
unsigned int bind_texture(std::string path);
utl::vector<unsigned int> set_texture_list(utl::vector<std::string> list);
void unbind_all();
}
