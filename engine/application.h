#pragma once
#include "core/program.h"
#include <memory>
struct GLFWwindow;

class application
{
public:
	bool Initialize();
	void Run();
	void Finalize();

	bool IsFinished() const { return _finished; }

private:
	unsigned int _texture;
	unsigned int _texture_specular;
	std::unique_ptr<program> _program;
	unsigned int _VAO;
	unsigned int _EBO;
	unsigned int _VBO;
	GLFWwindow* _window;
	bool _finished = false;
};
