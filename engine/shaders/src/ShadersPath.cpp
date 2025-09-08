#include "ShadersPath.h"
#include <filesystem>

namespace shaders
{
	std::string GetShadersPath()
	{
		return std::string(std::filesystem::path(__FILE__).remove_filename().string());
	}
}
