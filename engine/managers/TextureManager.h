#pragma once
#include "core/GLCommon.h"
#include "utility/vector.h"
#include <string>
namespace textures 
{

class texture_2d
{
public:
	void initialize(std::string path);
	void bind(unsigned int slot);
	void unbind();
	
	unsigned int get_id() const { return _id; }
private:
	bool binded = false;
	unsigned int _slot;

	unsigned int _id;
	std::string _path = "";
};


void add_texture(std::string path);
unsigned int bind_texture(std::string path);
utl::vector<unsigned int> set_texture_list(utl::vector<std::string> list);
void unbind_all();

texture_2d* get_texture(std::string path);

}
