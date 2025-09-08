#include "ecs.h"
#include "utility/vector.h"
#include "entity.h"
#include <iostream>
namespace ecs
{
	namespace {
		utl::vector<entity::entity, false, 1024> entity_data;
		utl::vector<unsigned int> generations;
	}

constexpr bool is_valid(entity::entity_id id )
{
	assert(id::is_valid(id));
	unsigned int index = id::index(id);

	assert(index < generations.size());

	return id::generation(id) == generations[index];
}

entity::entity_id create_entity()
{
	unsigned int index = entity_data.emplace_tombstone();	
	if ( entity_data.size() > generations.size() )
		generations.emplace_back(0);
	entity_data[index].create_transform();
	
	entity::entity_id id = id::set_generation(index, generations[index]);

	return id;
}

entity::entity* get_entity(entity::entity_id id)
{
	assert(is_valid(id));
	assert(id::generation(id) == generations[id::index(id)]);
	return &entity_data[id::index(id)];
}

void remove_entity(entity::entity_id id)
{
	unsigned int index = id::index(id);
	unsigned int generation = id::generation(id);

	assert(generations[index] == generation);

	entity_data[index].destroy();
	++ generations[index];
	
	entity_data.erase(entity_data.begin() + index);
}

void update()
{
	components::point_light::update();
}
	
}
