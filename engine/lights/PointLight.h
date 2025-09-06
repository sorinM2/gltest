#pragma once 
#include "glm/glm.hpp"
#include "utility/vector.h"
#include <string>
#include <unordered_set>
namespace lights 
{

class PointLight 
{
public:
	PointLight() = default;

	void initialize(unsigned int id) { _id = id; }

	void add_to_program(unsigned int id);
	void remove_from_program(unsigned int id);
	void update_programs();

	void set_position(const glm::vec3& position) { _position = position; }
	void set_ambient(const glm::vec3& ambient) { _ambient = ambient; }
	void set_diffuse(const glm::vec3& diffuse) { _diffuse = diffuse; }
	void set_specular(const glm::vec3& specular) { _specular = specular; }

	void set_constant(const float& constant) { _constant = constant; }
	void set_linear(const float& linear) { _linear = linear; }
	void set_quadratic(const float& quadratic) { _quadratic = quadratic; }

	glm::vec3 get_position() const { return _position; }
	glm::vec3 get_ambient() const { return _ambient; }
	glm::vec3 get_diffuse() const { return _diffuse; }
	glm::vec3 get_specular() const { return _specular; }

	float get_constant() const { return _constant; }
	float get_linear() const { return _linear; }
	float get_quadratic() const { return _quadratic; }
private:
	std::string get_uniform_name(const std::string& var_name) const;
	void internal_update_program(unsigned int id);
private:
	
	static std::string shader_array_name;

	unsigned int _id; 
	std::unordered_set<unsigned int> _programs;

	glm::vec3 _position{0., 0.f, 0.f};
	glm::vec3 _ambient{0.f, 0.f, 0.f};
	glm::vec3 _diffuse{0.f, 0.f, 0.f};
	glm::vec3 _specular{0.f, 0.f, 0.f};

	float _constant{1.f};
	float _linear{0.0f};
	float _quadratic{0.0f};
};

}
