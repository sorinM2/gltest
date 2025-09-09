#pragma once 
#include "core/common.h"
#include <string>
namespace ecs::entity 
{
class entity;
}

namespace ecs::components::geometry
{

class geometry 
{
public:
	geometry(entity::entity* entity, const std::string& model_path);	
private:
	entity::entity* _entity;
};

void create_geometry(entity::entity* entity, const std::string& model_path);
void remove_geometrt(geometry_id id);

geometry* get_geometry();
bool is_alive(geometry_id id);


}
