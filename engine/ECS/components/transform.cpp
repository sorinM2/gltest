#include "transform.h"
#include "glm/gtc/matrix_transform.hpp"
namespace ecs::components
{

void transform::set_position(const glm::vec3& position)
{
	_position = glm::translate(glm::mat4(1.f), position); 

	update_model();
}

void transform::set_rotation(const glm::vec3& rotation)
{
	_rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	_rotation = glm::rotate(_rotation, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	_rotation = glm::rotate(_rotation, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));

	update_model();
}

void transform::set_scale(const glm::vec3& scale)
{
	_scale = glm::scale(glm::mat4(1.f), scale);

	update_model();
}

void transform::update_model()
{
	_model = _position * _rotation * _scale;
}

transform::transform(entity* entity) : _entity{entity} { }

void transform::create()
{
	assert(!_active);
	if ( _active) return;

	_position = glm::mat4(1.f);
	_rotation = glm::mat4(1.f);
	_scale = glm::mat4(1.f);

	_active = true;
}

}
