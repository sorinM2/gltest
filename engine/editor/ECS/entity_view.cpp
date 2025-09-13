#include "entity_view.h"
#include "editor/editor_common.h"
#include "ECS/ecs.h"

namespace editor::entity 
{

namespace {
	ecs::entity::entity_id _selected_entity_id{id::invalid_id};
}

ecs::entity::entity_id get_selected_entity()
{
	return _selected_entity_id;
}

void update()
{
	auto entities = ecs::get_entity_vector();
	
	ImGui::Begin("entities");

	for ( auto& entity : entities )
	{
		std::string itemid = "##" + entity.get_name() + "_" + std::to_string(entity.get_id());
		if ( ImGui::Selectable(itemid.c_str(), entity.get_id() == _selected_entity_id) )
			_selected_entity_id = entity.get_id();
      		ImGui::SameLine();
		ImGui::Text("%s", entity.get_name().c_str());
	}

	ImGui::End();
}

}
