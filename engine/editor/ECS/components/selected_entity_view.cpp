#include "selected_entity_view.h"
#include "../entity_view.h"
#include "ECS/ecs.h"
#include "editor/editor_common.h"
#include "transform_view.h"

namespace editor::selected_entity 
{

void update()
{
	ImGui::Begin("entity view");	
	ecs::entity::entity_id id = entity::get_selected_entity();

	if ( id == id::invalid_id )
	{
		ImGui::End();
		return;
	}
	ecs::entity::entity* _entity = ecs::get_entity(id); 
	
	if ( _entity->get_transform() != nullptr )
		transform::update(_entity->get_transform());
	ImGui::End();
}

}
