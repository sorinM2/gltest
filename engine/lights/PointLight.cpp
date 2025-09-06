#include "PointLight.h"
#include "shaders/ProgramManager.h"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
namespace lights
{

std::string PointLight::shader_array_name = "pointLights";

std::string PointLight::get_uniform_name(const std::string& var_name) const 
{
	std::string struct_name = shader_array_name + "[" + std::to_string(_id) + "].";
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

}
