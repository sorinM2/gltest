#include "AssetsPath.h"
#include <filesystem>

namespace assets
{
	std::string GetAssetsPath()
	{
		return std::string(std::filesystem::path(__FILE__).remove_filename().c_str());
	}
}
