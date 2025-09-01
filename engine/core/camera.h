#pragma once

#include "core/common.h"
#include "glm/ext/matrix_float4x4.hpp"

namespace camera
{
	void Initialize(GLFWwindow* win);
	void Update();
	glm::mat4 GetViewMatrix();
}
