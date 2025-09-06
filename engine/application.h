#pragma once
#include "shaders/program.h"
#include <memory>
#include "lights/PointLight.h"
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
	programs::program* _program;
	unsigned int _VAO;
	unsigned int _EBO;
	unsigned int _VBO;
	GLFWwindow* _window;
	bool _finished = false;

	lights::PointLight light1;
	lights::PointLight light2;
};
