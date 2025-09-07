#pragma once 
#include "core/common.h"
#include "components/transform.h"
#include "components/lights/point_light.h"

namespace ecs::entity 
{


class entity 
{
public:
	entity();
	components::transform::transform* get_transform();
	components::point_light::point_light* get_point_light();

	void create_transform();
	void create_point_light(); 
	

	void destroy();
private:
	unsigned int _transform{id::invalid_id};
	unsigned int _point_light{id::invalid_id};
};

}
