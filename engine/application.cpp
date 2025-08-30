#include "application.h"
#include "shaders/ShadersPath.h"
#include <fstream>
#include <sstream>
#include "utility/unmove.h"

#define GLAD_GL_IMPLEMENTATION
#include "glad/glad.h"

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

float vertices[] = 
{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

bool application::Initialize()
{
	if ( ! glfwInit() )
	{
		std::cout << "GLFW initialization failed!" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(800, 800, "fereastra", NULL, NULL);

	if ( ! _window )
	{
		std::cout << "Failed to create window!" << std::endl;
		return false;
	}
	glfwSetKeyCallback(_window, key_callback);		
	glfwMakeContextCurrent(_window);

	int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glGenBuffers(1, &_VBO);


	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	std::cout << "Shader path: " << shaders::GetShadersPath() << std::endl;

 	std::ifstream vertex(shaders::GetShadersPath() + "vertex.glsl");
	{
		std::stringstream buffer;
		buffer << vertex.rdbuf();

		//std:: cout << "Vertex Shader code:\n" << buffer.str() << std::endl;

		_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		
		glShaderSource(_vertexShader, 1, &util::unmove(buffer.str().c_str()), NULL);
		glCompileShader(_vertexShader);
	}

	int success;
	char infoLog[512];
	glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &success);

	if ( !success )
	{
		glGetShaderInfoLog(_vertexShader, 512, NULL, infoLog);
		std::cout << "Error compilating vertex shader!" << std::endl;
		std::cout << infoLog << std::endl;
	}
	vertex.close();

	std::ifstream fragment(shaders::GetShadersPath() + "fragment.glsl");

	{
		std::stringstream buffer;
		buffer << fragment.rdbuf();

		//std:: cout << "Vertex Shader code:\n" << buffer.str() << std::endl;

		_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		glShaderSource(_fragmentShader, 1, &util::unmove(buffer.str().c_str()), NULL);
		glCompileShader(_fragmentShader);
	}	

	glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &success);

	if ( !success )
	{
		glGetShaderInfoLog(_fragmentShader, 512, NULL, infoLog);
		std::cout << "Error compiling fragment shader!" << std::endl;
		std::cout << infoLog << std::endl;
	}

	_program = glCreateProgram();
	glAttachShader(_program, _vertexShader);
	glAttachShader(_program, _fragmentShader);
	glLinkProgram(_program);

	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if ( !success )
	{
		glGetProgramInfoLog(_program, 512, NULL, infoLog);
		std::cout << "Error linking program!" << std::endl;
		std::cout << infoLog << std::endl;
	}

	glUseProgram(_program);

	glDeleteShader(_vertexShader);
	glDeleteShader(_fragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 *  sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return true;
}


void application::Run()
{
	if ( glfwWindowShouldClose(_window))
		_finished = true;

	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.3f, 0.8f, 0.3f, 1.f);
	

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(_program);
	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(_window);
	glfwPollEvents();


}

void application::Finalize()
{ 
	glfwTerminate();
}

