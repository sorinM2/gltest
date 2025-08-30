#pragma once
#include "GLFW/glfw3.h"

class application
{
public:
	bool Initialize();
	void Run();
	void Finalize();

	bool IsFinished() const { return _finished; }

private:
	GLFWwindow* _window;
	bool _finished = false;
};
