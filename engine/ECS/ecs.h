#pragma once
#include "ecscommon.h"
#include "entity.h"

namespace ecs 
{

constexpr bool is_valid(entity::entity_id id );

entity::entity_id create_entity();
void remove_entity(entity::entity_id id);

entity::entity* get_entity(entity::entity_id id);

void update();
}
