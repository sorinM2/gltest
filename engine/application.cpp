#include "application.h"
#include "assets/AssetsPath.h"
#include "core/common.h"
#include "core/GLCommon.h"

#include "managers/ProgramManager.h"
#include "shaders/src/ShadersPath.h"

#include "core/camera.h"
#include "ECS/ecs.h"

#include "managers/TextureManager.h"

#include "glm/gtc/type_ptr.hpp"

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
float vertices[] = {
// positions // normals // texture coords
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

int indices[] = 
{
	0, 1, 2,
	0, 2, 3
};

glm::vec3 cubePositions[] = {
glm::vec3( 0.0f, 0.0f, -1.0f),
glm::vec3( 2.0f, 5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3( 2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f, 3.0f, -7.5f),
glm::vec3( 1.3f, -2.0f, -2.5f),
glm::vec3( 1.5f, 2.0f, -2.5f),
glm::vec3( 1.5f, 0.2f, -1.5f),
glm::vec3(-1.3f, 1.0f, -1.5f)
};

bool application::Initialize()
{

	spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

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

	glfwSwapInterval(0);
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
	
	unsigned int prog = programs::AddProgram();
        _program = programs::GetProgram(prog);

        _program->AddShader(programs::program::VERTEX,
                            shaders::GetShadersPath() + "vertex.glsl");
        _program->AddShader(programs::program::FRAGMENT,
                            shaders::GetShadersPath() + "fragment.glsl");
        _program->Link();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *  sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 *  sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);
	
	_texture =assets::GetAssetsPath() + "resources/textures/container2.png";
	_texture_specular = assets::GetAssetsPath() + "resources/textures/container2_specular.png";

	textures::add_texture(_texture, GL_RGBA, GL_RGBA);
	textures::add_texture(_texture_specular, GL_RGBA, GL_RGBA);

	glEnable(GL_DEPTH_TEST);

	camera::Initialize(_window);
	
		
	entt = ecs::create_entity();
	ecs::entity::entity* _entity = ecs::get_entity(entt);

	_entity->create_point_light();
	
	ecs::remove_entity(entt);

	entt = ecs::create_entity();
	_entity = ecs::get_entity(entt);

	_entity->create_point_light();

	ecs::components::point_light::point_light* point = _entity->get_point_light();

	point->set_ambient( glm::vec3(0.1f, 0.1f, 0.1f));
	point->set_diffuse(glm::vec3(1.f, 1.f, 1.f));
	point->set_specular(glm::vec3(10.f, 10.f, 10.f));
	point->add_to_program(prog);

	unsigned int tex_specular_slot = textures::bind_texture(_texture_specular);
	unsigned int tex_slot = textures::bind_texture(_texture);

	_program->SetUniform1i("material.ambient", tex_slot);
	_program->SetUniform1i("material.specular", tex_specular_slot);
	return true;
}


void application::Run()
{
	if ( glfwWindowShouldClose(_window))
		_finished = true;
	cubePositions[0].x = sin(glfwGetTime() / 4) * 5;
	cubePositions[0].z = cos(glfwGetTime() / 4) * 5;
	cubePositions[0].y = sin(glfwGetTime() / 4)* cos(glfwGetTime() / 4) * 5;

	ecs::entity::entity* _entity = ecs::get_entity(entt);
	_entity->get_transform()->set_position(cubePositions[0]);
	ecs::update();
	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	
	glBindVertexArray(_VAO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_program->Bind();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float )width / (float) height, 0.1f, 100.0f);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::inverse(camera::GetViewMatrix());

	
	_program->SetUniformMatrix4fv("projection_view", false, glm::value_ptr(projection * view));
	
	


	_program->SetUniform1f("material.diffuse", 0.5f);
	_program->SetUniform1i("material.shininess", 1024);
	camera::Update();
	_program->SetUniform3fv("eyePos", glm::value_ptr(camera::GetCameraPos()));
	for ( unsigned int i = 0; i < 10; ++i )
	{
		glm::mat4 model = glm::mat4(1.f);
		_program->SetUniform1i("isLight", 0);
		if ( i == 0 )
		{
			//cubePositions[i].x = sin(glfwGetTime() / 4) * 5;
			//cubePositions[i].z = cos(glfwGetTime() / 4) * 5;
			//cubePositions[i].y = sin(glfwGetTime() / 4)* cos(glfwGetTime() / 4) * 5;
			model = _entity->get_transform()->get_model();
			_program->SetUniform1i("isLight", 1);
		}
		else 
		{
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians((float)glfwGetTime() * i), glm::vec3(1.f,1.f, 1.f));
			model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		}
		_program->SetUniformMatrix4fv("model", false, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glfwSwapBuffers(_window);
	glfwPollEvents();


}

void application::Finalize()
{ 
	glDeleteProgram(_program->GetId());
	glfwTerminate();
}

