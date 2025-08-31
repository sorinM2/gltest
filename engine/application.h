#pragma once
#include "core/shader.h"
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
	unsigned int _program;
	unsigned int _VAO;
	unsigned int _EBO;
	std::unique_ptr<shader> _vertexShader{nullptr};
	std::unique_ptr<shader> _fragmentShader{nullptr};
	unsigned int _VBO;
	GLFWwindow* _window;
	bool _finished = false;
};
