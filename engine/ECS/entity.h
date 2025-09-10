#pragma once 
#include "core/common.h"
#include "components/transform.h"
#include "components/lights/point_light.h"
#include "components/geometry.h"
#include "shaders/program.h"
namespace ecs::entity 
{


class entity 
{
friend class components::transform::transform;
friend class components::point_light::point_light;
friend class components::geometry::geometry;

public:
	entity();
	components::transform::transform* get_transform();
	components::point_light::point_light* get_point_light();
	components::geometry::geometry* get_geometry();
	
	void create_geometry(const std::string& model_path, programs::program* program, bool texture_flipped = false);
	void create_transform();
	void create_point_light(); 

	void update();

	void destroy();
private:
	components::transform::transform_id _transform{id::invalid_id};
	components::point_light::point_light_id _point_light{id::invalid_id};
	components::geometry::geometry_id _geometry{id::invalid_id};
};

}
