#pragma once 
#include "core/GLCommon.h"
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

	void set_position(const glm::vec3& position);
	void set_ambient(const glm::vec3& ambient);
	void set_diffuse(const glm::vec3& diffuse);
	void set_specular(const glm::vec3& specular);

	void set_constant(const float& constant);
	void set_linear(const float& linear);
	void set_quadratic(const float& quadratic);
	
	void set_active(bool value);
	glm::vec3 get_position() const { return _position; }
	glm::vec3 get_ambient() const { return _ambient; }
	glm::vec3 get_diffuse() const { return _diffuse; }
	glm::vec3 get_specular() const { return _specular; }

	float get_constant() const { return _constant; }
	float get_linear() const { return _linear; }
	float get_quadratic() const { return _quadratic; }

	bool get_active() const { return _active;}
private:
	std::string get_uniform_name(const std::string& var_name) const;
	void internal_update_program(unsigned int id);
private:
	std::string parent_light = "";	
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

	bool _active{true};
};

}
