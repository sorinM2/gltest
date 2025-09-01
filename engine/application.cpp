#include "application.h"
#include "assets/AssetsPath.h"
#include "core/common.h"
#include "core/program.h"
#include "shaders/ShadersPath.h"
#include <iostream>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"
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
	//positions			//colors		//texture coords
	-0.5f, -0.5f, 0.0f,		1.f, 0.f, 0.f,		0.f, 0.f,
	0.5f, -0.5f, 0.0f,		0.f, 1.f, 0.f,		1.f, 0.f,
	0.5f, 0.5f, 0.0f,		0.f, 0.f, 1.f,		1.f, 1.f,
	-0.5f, 0.5f, 0.0f,		1.f, 1.f, 1.f,		0.f, 1.f,
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

        _program = std::make_unique<program>();
        _program->AddShader(program::VERTEX,
                            shaders::GetShadersPath() + "vertex.glsl");
        _program->AddShader(program::FRAGMENT,
                            shaders::GetShadersPath() + "fragment.glsl");
        _program->Link();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *  sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 *  sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);
	
	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;
	unsigned char* data = stbi_load((assets::GetAssetsPath() + "textures/fragile_box.jpg").c_str(), &width, &height, &channels, 0);
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	
	stbi_image_free(data);
	return true;
}


void application::Run()
{
	if ( glfwWindowShouldClose(_window))
		_finished = true;

	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.8f, 0.3f, 0.3f, 1.f);
	

	glClear(GL_COLOR_BUFFER_BIT);
	_program->Bind();
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(_window);
	glfwPollEvents();


}

void application::Finalize()
{ 
	glfwTerminate();
}

