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
	GLFWwindow* _window;
	bool _finished = false;
};
