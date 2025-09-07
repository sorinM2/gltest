#pragma once 
#include "glm/glm.hpp" 
#include "core/common.h"

namespace ecs 
{
	class entity;
}

namespace ecs::components 
{
class transform
{
public:
	friend class ecs::entity;
	
	void create();

	void set_position(const glm::vec3& position);
	void set_rotation(const glm::vec3& rotation);
	void set_scale(const glm::vec3& scale);

	glm::mat4 get_model() const { return _model; }

	bool is_active() const { return _active; }
private:
	void update_model();

	transform(entity* entity);
private:
	glm::mat4 _model;

	glm::mat4 _position;
	glm::mat4 _rotation;
	glm::mat4 _scale;
	
	entity* _entity;
	bool _active = false;
};

}
