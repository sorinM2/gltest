#include "application.h"
#include "GLFW/glfw3.h"
#include <iostream>

void error_callback(int error, const char* description)
{	
	std::cout << "Error: " << description <<std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

bool application::Initialize()
{
	if ( ! glfwInit() )
	{
		std::cout << "GLFW initialization failed!" << std::endl;
		return false;
	}
	
	_window = glfwCreateWindow(800, 800, "fereastra", NULL, NULL);

	if ( ! _window )
	{
		std::cout << "Failed to create window!" << std::endl;
		return false;
	}
	glfwSetKeyCallback(_window, key_callback);		
	glfwMakeContextCurrent(_window);


	return true;
}

void application::Run()
{
	if ( glfwWindowShouldClose(_window))
		_finished = true;

	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	

	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(_window);
	glfwPollEvents();


}

void application::Finalize()
{ 
	glfwTerminate();
}

