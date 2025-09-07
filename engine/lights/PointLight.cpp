#include "PointLight.h"
#include "managers/ProgramManager.h"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
namespace lights
{

std::string PointLight::shader_array_name = "pointLights";

std::string PointLight::get_uniform_name(const std::string& var_name) const 
{
	std::string struct_name = "";
	if ( parent_light == "" )
		struct_name = shader_array_name + "[" + std::to_string(_id) + "].";
	else struct_name = parent_light + var_name;
	return struct_name + var_name;
}

void PointLight::internal_update_program(unsigned int id)
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

void PointLight::update_programs()
{
	for ( auto program : _programs )
		internal_update_program(program);
}

void PointLight::add_to_program(unsigned int id)
{
	std::string active_name = get_uniform_name("is_active");
	programs::program* prog = programs::GetProgram(id);

	prog->SetUniform1i(active_name, true);
	_programs.emplace(id);
	internal_update_program(id);
}

void PointLight::remove_from_program(unsigned int id)
{
	std::string active_name = get_uniform_name("is_active");
	programs::program* prog = programs::GetProgram(id);
	prog->SetUniform1i(active_name, false);
	_programs.erase(id);
}
	

void PointLight::set_position(const glm::vec3& position)
{
	_position = position; 
	std::string position_name = get_uniform_name("position");

	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform3fv(position_name, glm::value_ptr(_position));
	}
}

void PointLight::set_ambient(const glm::vec3& ambient) 
{
	_ambient = ambient;
	std::string ambient_name = get_uniform_name("ambient");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform3fv(ambient_name, glm::value_ptr(_ambient));
	}
}

void PointLight::set_diffuse(const glm::vec3& diffuse) 
{
	_diffuse = diffuse; 
	std::string diffuse_name = get_uniform_name("diffuse");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform3fv(diffuse_name, glm::value_ptr(_diffuse));
	}
}

void PointLight::set_specular(const glm::vec3& specular) 
{
	_specular = specular; 
	std::string specular_name = get_uniform_name("specular");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform3fv(specular_name, glm::value_ptr(_specular));
	}
}

void PointLight::set_constant(const float& constant) 
{
	_constant = constant; 
	std::string constant_name = get_uniform_name("constant");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform1f(constant_name, _constant);
	}
}

void PointLight::set_linear(const float& linear) 
{
	_linear = linear; 
	std::string linear_name = get_uniform_name("linear");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform1f(linear_name, _linear);
	}
}

void PointLight::set_quadratic(const float& quadratic) 
{
	_quadratic = quadratic; 
	std::string quadratic_name = get_uniform_name("quadratic");
	for ( auto id : _programs)
	{
		programs::program* prog = programs::GetProgram(id);
		prog->SetUniform1f(quadratic_name, _quadratic);
	}
}

void PointLight::set_active(bool value)
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
