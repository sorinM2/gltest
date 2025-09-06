#pragma once
#include <string>
#include "core/common.h"

class shader 
{
public:
	shader(const std::string& path, GLenum shader_type);
	std::string GetPath() const { return _path; }
	unsigned int GetId() const { return _id; }
	bool IsActive() const { return _active; }

	void Destroy();
private:
	unsigned int _id = 0;
	std::string _path;

	bool _active = true;
};
