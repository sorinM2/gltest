#include "application.h"
#include "core/common.h"
#include "shaders/ShadersPath.h"
#include "core/shader.h"
#include <iostream>
#include <memory>

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
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};

int indices[] = 
{
	0, 1, 2,
	0, 2, 3
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

	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	std::cout << "Shader path: " << shaders::GetShadersPath() << std::endl;

	
	_vertexShader = std::make_unique<shader>(shaders::GetShadersPath() + "vertex.glsl", GL_VERTEX_SHADER);
	_fragmentShader = std::make_unique<shader>(shaders::GetShadersPath() + "fragment.glsl", GL_FRAGMENT_SHADER);

	_program = glCreateProgram();
	glAttachShader(_program, _vertexShader->GetId());
	glAttachShader(_program, _fragmentShader->GetId());
	glLinkProgram(_program);

	int success;
	char infoLog[512];
	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if ( !success )
	{
		glGetProgramInfoLog(_program, 512, NULL, infoLog);
		std::cout << "Error linking program!" << std::endl;
		std::cout << infoLog << std::endl;
	}

	glUseProgram(_program);

	_vertexShader->Destroy();
	_fragmentShader->Destroy();

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
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(_window);
	glfwPollEvents();


}

void application::Finalize()
{ 
	glfwTerminate();
}

