#include "application.h"
#include "core/GLCommon.h"

#include "ECS/ecs.h"
#include "managers/ProgramManager.h"
#include "managers/TextureManager.h"

#include "shaders/src/ShadersPath.h"
#include "assets/AssetsPath.h"

#include "core/camera.h"

#include "glm/gtc/type_ptr.hpp"
#include "content/scene.h"

#include "editor/editor.h"
#include "utility/util.h"

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
	
	editor::editor_init_data editor_descriptor;
	editor_descriptor._window = _window;
	editor_descriptor._scene_view_data._frame_buffer_height = 1080;
	editor_descriptor._scene_view_data._frame_buffer_width = 1920;

	editor::initialize(editor_descriptor);
	
	unsigned int prog = programs::AddProgram();
        _program = programs::GetProgram(prog);

        _program->AddShader(programs::program::VERTEX,
                            shaders::GetShadersPath() + "vertex.glsl");
        _program->AddShader(programs::program::FRAGMENT,
                            shaders::GetShadersPath() + "fragment.glsl");
        _program->Link();

	_scene = assets::GetAssetsPath() + "resources/objects/rock/rock.obj";
	_scene2 = assets::GetAssetsPath() + "resources/objects/cro/cro.glb";

	

	glEnable(GL_DEPTH_TEST);

	camera::Initialize(_window);
	
		

	for ( unsigned int i = 0; i < 9; ++i )
	{
		std::cout << "debug start" << std::endl;
		entt.emplace_back(ecs::create_entity("croissant_" + std::to_string(i)));
		auto _entity = ecs::get_entity(entt[i]);
		_entity->create_geometry(_scene2, _program);
		_entity->get_transform()->set_position(glm::vec3((i % 3) * 4, 0.f, i / 3 * 4));
		_entity->get_transform()->set_scale(glm::vec3(0.1f, 0.1f, 0.1f));
		_entity->get_transform()->set_rotation(glm::vec3(-90.f, 0.f, 180.f));
		std::cout << "debug stop" << std::endl;
	}



	_program->SetUniform3fv("dirLight.direction", glm::value_ptr(glm::vec3(-0.5f, -1.f, 0.3f)));
	_program->SetUniform3fv("dirLight.ambient", glm::value_ptr(glm::vec3(0.23f, 0.23f, 0.23f)));
	_program->SetUniform3fv("dirLight.diffuse", glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
	_program->SetUniform3fv("dirLight.specular", glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));


	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	return true;
}

void application::Run()
{
	if ( glfwWindowShouldClose(_window))
		_finished = true;
	
	editor::start_frame();

	_program->Bind();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float )1920 / (float)1080, 0.1f, 1000.0f);

	glm::mat4 view = glm::mat4(1.f);
	view = camera::GetViewMatrix();
	_program->SetUniformMatrix4fv("projection_view", false, glm::value_ptr(projection * view));
	

	_program->SetUniform1f("material.diffuse", 1.f);
	_program->SetUniform1i("material.shininess", 1024);
	camera::Update();
	_program->SetUniform3fv("eyePos", glm::value_ptr(camera::GetCameraPos()));
	
	editor::scene::bind_framebuffer();

	ecs::update();

	editor::update();
	editor::draw();

	glfwSwapBuffers(_window);
	glfwPollEvents();


}

void application::Finalize()
{ 
	glDeleteProgram(_program->GetId());
	glfwTerminate();
}

