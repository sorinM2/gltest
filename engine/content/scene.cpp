#include "scene.h"
#include <unordered_map>
#include "content_loader.h"
#include "utility/path.h"
namespace content::scene 
{

namespace {
	std::unordered_map<std::string, scene> scenes;
}

void scene::draw(programs::program* prog, glm::mat4 global_model)
{
	unsigned int i = 0;
	for ( auto c : _models )
	{
		model::model* m = model::get_model(c);
		m->draw(prog, global_model);
	}
}

void create_scene(std::string path, bool uv_flipped)
{
	utl::normalize_path(path);
	if ( scenes.find(path) != scenes.end() )
		return;
	scenes[path] = create_scene_from_file(path, uv_flipped);		
}

scene* get_scene(std::string path)
{
	utl::normalize_path(path);
	return &scenes[path];
}


}
