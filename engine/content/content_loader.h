#pragma once
#include <string>

namespace content::scene
{
	class scene;
}

namespace content 
{
scene::scene create_scene_from_file(std::string path, bool uv_flipped = true);
}
