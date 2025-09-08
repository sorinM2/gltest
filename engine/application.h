#pragma once
#include "shaders/program.h"
#include "core/GLCommon.h"
#include "core/common.h"
#include <memory>
#include "ECS/entity.h"

struct GLFWwindow;

class application
{
public:
	bool Initialize();
	void Run();
	void Finalize();

	bool IsFinished() const { return _finished; }

private:
	std::string _texture;
	std::string _texture_specular;
	programs::program* _program;
	unsigned int _VAO;
	unsigned int _EBO;
	unsigned int _VBO;
	GLFWwindow* _window;
	bool _finished = false;
	
	ecs::entity::entity_id entt;

};
