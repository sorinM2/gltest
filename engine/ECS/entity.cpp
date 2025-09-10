#include "entity.h"
#include "shaders/program.h"
namespace ecs::entity 
{

entity::entity()
{
}

void entity::create_transform()
{
	_transform = components::transform::create_transform(this);
}

void entity::destroy()
{
	if ( components::transform::is_valid(_transform) )
		components::transform::delete_transform(_transform);
	if ( components::point_light::is_valid(_point_light) )
		components::point_light::delete_point_light(_point_light);
}

components::transform::transform* entity::get_transform()
{
	return components::transform::get_transform(_transform);
}

void entity::create_point_light()
{
	_point_light = components::point_light::create_point_light(this);
}

components::point_light::point_light* entity::get_point_light()
{
	return components::point_light::get_point_light(_point_light);
}

void entity::create_geometry(const std::string& model_path, programs::program* program, bool textrue_flipped)
{
	_geometry = components::geometry::create_geometry(this, model_path, program, textrue_flipped);
}

components::geometry::geometry* entity::get_geometry()
{
	return components::geometry::get_geometry(_geometry);
}

}
