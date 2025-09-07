#include "ecs.h"
#include "utility/vector.h"
#include "entity.h"
namespace ecs
{

utl::vector<entity, false, 1024> entity_data;
utl::vector<unsigned int> generations;

constexpr bool is_alive(entity_id id )
{
	assert(id::is_valid(id));
	unsigned int index = id::index(id);

	assert(index < generations.size());

	return id::generation(id) == generations[index];
}

entity_id create_entity()
{
	unsigned int index = entity_data.emplace_tombstone();	
	if ( entity_data.size() > generations.size() )
		generations.emplace_back(0);
	entity_data[index].create_transform();
	
	assert(entity_data[index].get_transform()->is_active());
	
	entity_id id = id::set_generation(index, generations[index]);

	#if _DEBUG
	std::cout << "created entity with generation: " << id::generation(id) << ", and index: " << id::index(id) << std::endl;
	#endif

	return id;
}

entity* get_entity(entity_id id)
{
	assert(is_alive(id));
	return &entity_data[id::index(id)];
}

void remove_entity(entity_id id)
{
	unsigned int index = id::index(id);
	unsigned int generation = id::generation(id);

	assert(generations[index] == generation);
	++ generations[index];
	
	entity_data.erase(entity_data.begin() + index);
}
	
}
