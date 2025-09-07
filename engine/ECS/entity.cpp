#include "entity.h"

namespace ecs 
{

entity::entity() : _transform(this)
{
}

void entity::create_transform()
{
	_transform.create();	
}

components::transform* entity::get_transform()
{
	return _transform.is_active()? &_transform: nullptr; 
}

}
