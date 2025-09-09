#pragma once
#include "model.h"
#include "utility/vector.h"
#include "shaders/program.h"
namespace content::scene 
{

struct scene 
{
	utl::vector<unsigned int> _models;	

	void draw(programs::program* prog, glm::mat4 global_model);
};

void create_scene(std::string path, bool uv_flipped = true);
scene* get_scene(std::string path);
}
