#pragma once 
#include "core/common.h"
#include "components/transform.h"


namespace ecs 
{

DEFINE_TYPED_ID(entity);

class entity 
{
public:
	entity();
	components::transform* get_transform();
	void create_transform();
private:
	components::transform _transform;
};

}
