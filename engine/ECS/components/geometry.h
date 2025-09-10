#pragma once 
#include "core/common.h"
#include <string>
#include "shaders/program.h"
namespace ecs::entity 
{
class entity;
}

namespace ecs::components::geometry
{

class geometry 
{
public:
	geometry(entity::entity* entity, const std::string& model_path, programs::program* program, bool texture_flipped = false);	
	void draw();
private:
	programs::program* _program;
	entity::entity* _entity;
	std::string path = "";
};

geometry_id create_geometry(entity::entity* entity, const std::string& model_path, programs::program* program, bool texture_flipped = false);
void remove_geometry(geometry_id id);

geometry* get_geometry(geometry_id id);
bool is_alive(geometry_id id);

void draw();

}
