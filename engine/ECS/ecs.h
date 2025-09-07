#pragma once
#include "ecscommon.h"
#include "entity.h"

namespace ecs 
{
constexpr bool is_alive(entity_id id );

entity_id create_entity();
void remove_entity(entity_id id);

entity* get_entity(entity_id id);

}
