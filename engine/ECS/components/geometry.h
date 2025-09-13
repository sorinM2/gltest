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
	geometry(entity::entity_id entity, const std::string& model_path, programs::program* program, bool texture_flipped = false);	
	void draw();
private:
	programs::program* _program;
	entity::entity_id _entity_id;
	std::string path = "";
};

geometry_id create_geometry(entity::entity_id entity, const std::string& model_path, programs::program* program, bool texture_flipped = false);
void remove_geometry(geometry_id id);

geometry* get_geometry(geometry_id id);
bool is_valid(geometry_id id);

void draw();

}
