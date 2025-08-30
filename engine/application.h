#pragma once

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
	unsigned int _vertexShader = 0;
	unsigned int _fragmentShader = 0;
	unsigned int _VBO;
	GLFWwindow* _window;
	bool _finished = false;
};
