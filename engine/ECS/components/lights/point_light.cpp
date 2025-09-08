#include "point_light.h"
#include "managers/ProgramManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "utility/vector.h"
#include "ECS/components/transform.h"
#include <iostream>
#include "ECS/entity.h"

namespace ecs::components::point_light
{
	namespace{
		utl::vector<point_light, false> point_lights;
		utl::vector<unsigned int> generations;  
	}

point_light_id create_point_light(entity::entity* entity)
{
	unsigned int index = point_lights.emplace_tombstone(entity);
	point_lights[index].initialize(index);

   	if ( point_lights.size() > generations.size() )
		generations.emplace_back(0);
	point_light_id id = id::set_generation(index, generations[index]); 

	return id;
}

void delete_point_light(point_light_id id)
{
	assert(id::generation(id) == generations[id::index(id)]);
	
	point_lights.erase(point_lights.begin() + id::index(id));
}

point_light* get_point_light(point_light_id id )
{
	assert(id::generation(id) == generations[id::index(id)]);

	return &point_lights[id::index(id)];
}

bool is_valid(point_light_id id)
{
	assert(id::is_valid(id));
	return id::generation(id) == generations[id::index(id)];
}

void update()
{
	for ( auto& light : point_lights )
		light.update();
}

point_light::point_light(entity::entity* entity): _entity{entity}{
	_transform = _entity->get_transform();
}

void point_light::initialize(point_light_id id)
{
	assert(id::is_valid(id));
	_id = id;
}

void point_light::update()
{
	_transform = _entity->get_transform();
	glm::vec3 tranform_position = _transform->get_position();
	if ( tranform_position != _position )
		set_position(tranform_position);
}

std::string point_light::shader_array_name = "pointLights";

std::string point_light::get_uniform_name(const std::string& var_name) const 
{
	std::string struct_name = "";
	if ( parent_light == "" )
		struct_name = shader_array_name + "[" + std::to_string(_id) + "].";
	else struct_name = parent_light + var_name;
	return struct_name + var_name;
}

void point_light::internal_update_program(unsigned int id)
{
	std::string position_name = get_uniform_name("position");
	std::string ambient_name = get_uniform_name("ambient");
	std::string diffuse_name = get_uniform_name("diffuse");
	std::string specular_name = get_uniform_name("specular");
	
	std::string constant_name = get_uniform_name("constant");
	std::string linear_name = get_uniform_name("linear");
	std::string quadratic_name = get_uniform_name("quadratic");

	
	programs::program* prog = programs::GetProgram(id);
	prog->SetUniform3fv(position_name, glm::value_ptr(_position));
	prog->SetUniform3fv(ambient_name, glm::value_ptr(_ambient));
	prog->SetUniform3fv(diffuse_name, glm::value_ptr(_diffuse));
	prog->SetUniform3fv(specular_name, glm::value_ptr(_specular));
	
	prog->SetUniform1f(constant_name, _constant);
	prog->SetUniform1f(linear_name, _linear);
	prog->SetUniform1f(quadratic_name, _quadratic);


}

void point_light::update_programs()
{
	for ( auto program : _programs )
		internal_update_program(program);
}

void point_light::add_to_program(unsigned int id)
{
	std::string active_name = get_uniform_name("is_active");
	programs::program* prog = programs::GetProgram(id);

	prog->SetUniform1i(active_name, true);
	_programs.emplace(id);
	internal_update_program(id);
}

void point_light::remove_from_program(unsigned int id)
{
	std::string active_name = get_uniform_name("is_active");
	programs::program* prog = programs::GetProgram(id);
	prog->SetUniform1i(active_name, false);
	_programs.erase(id);
}
	

void point_light::set_position(const glm::vec3& position)
{
	_position = position; 
	std::string position_name = get_uniform_name("position");

	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform3fv(position_name, glm::value_ptr(_position));
	}
}

void point_light::set_ambient(const glm::vec3& ambient) 
{
	_ambient = ambient;
	std::string ambient_name = get_uniform_name("ambient");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform3fv(ambient_name, glm::value_ptr(_ambient));
	}
}

void point_light::set_diffuse(const glm::vec3& diffuse) 
{
	_diffuse = diffuse; 
	std::string diffuse_name = get_uniform_name("diffuse");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform3fv(diffuse_name, glm::value_ptr(_diffuse));
	}
}

void point_light::set_specular(const glm::vec3& specular) 
{
	_specular = specular; 
	std::string specular_name = get_uniform_name("specular");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform3fv(specular_name, glm::value_ptr(_specular));
	}
}

void point_light::set_constant(const float& constant) 
{
	_constant = constant; 
	std::string constant_name = get_uniform_name("constant");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform1f(constant_name, _constant);
	}
}

void point_light::set_linear(const float& linear) 
{
	_linear = linear; 
	std::string linear_name = get_uniform_name("linear");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform1f(linear_name, _linear);
	}
}

void point_light::set_quadratic(const float& quadratic) 
{
	_quadratic = quadratic; 
	std::string quadratic_name = get_uniform_name("quadratic");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform1f(quadratic_name, _quadratic);
	}
}

void point_light::set_active(bool value)
{
	_active = value;
	std::string active_name = get_uniform_name("is_active");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform1i(active_name, _active);
	}
}

}
