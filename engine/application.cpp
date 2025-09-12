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

	editor::initialize(_window);
	
	unsigned int prog = programs::AddProgram();
        _program = programs::GetProgram(prog);

        _program->AddShader(programs::program::VERTEX,
                            shaders::GetShadersPath() + "vertex.glsl");
        _program->AddShader(programs::program::FRAGMENT,
                            shaders::GetShadersPath() + "fragment.glsl");
        _program->Link();

	_scene = assets::GetAssetsPath() + "resources/objects/rock/rock.obj";
	_scene2 = assets::GetAssetsPath() + "resources/objects/bred/bread.glb";
	

	glEnable(GL_DEPTH_TEST);

	camera::Initialize(_window);
	
		
	entt = ecs::create_entity();
	ecs::entity::entity* _entity = ecs::get_entity(entt);

	_entity->create_point_light();
	_entity->create_geometry(_scene2, _program);

	ecs::components::point_light::point_light* point = _entity->get_point_light();

	point->set_position(glm::vec3(0.f, 1.f, 0.f));

	point->set_ambient( glm::vec3(0.1f, 0.1f, 0.1f));
	point->set_diffuse(glm::vec3(0.3f, 0.3f, 0.3f));
	point->set_specular(glm::vec3(1.f, 1.f, 1.f));
	
	point->add_to_program(prog);

	_program->SetUniform3fv("dirLight.direction", glm::value_ptr(glm::vec3(-0.5f, -1.f, 0.3f)));
	_program->SetUniform3fv("dirLight.ambient", glm::value_ptr(glm::vec3(0.23f, 0.23f, 0.23f)));
	_program->SetUniform3fv("dirLight.diffuse", glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
	_program->SetUniform3fv("dirLight.specular", glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));

	_entity->get_transform()->set_position(glm::vec3(0.f, 0.f, 0.f));
	_entity->get_transform()->set_rotation(glm::vec3(180.f, 0.f, 0.f));

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	_framebuf = new data::FrameBuffer(960, 540, true);
	_framebuf->add_texture_2d("culori", GL_RGB, GL_COLOR_ATTACHMENT0);
	_framebuf->add_renderbuffer("depth", GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);

	return true;
}

unsigned int width = 960, height = 540;

void application::Run()
{
	if ( glfwWindowShouldClose(_window))
		_finished = true;
	
	editor::start_frame();

	_framebuf->bind();
	_framebuf->clear();

	ecs::entity::entity* _entity = ecs::get_entity(entt);

	_program->Bind();

	glm::mat4 projection;

	projection = glm::perspective(glm::radians(45.0f), (float )_framebuf->get_width() / (float)_framebuf->get_height(), 0.1f, 1000.0f);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::inverse(camera::GetViewMatrix());

	
	_program->SetUniformMatrix4fv("projection_view", false, glm::value_ptr(projection * view));
	
	content::scene::scene* sc = content::scene::get_scene(_scene);
	content::scene::scene* sc2 = content::scene::get_scene(_scene2);


	_program->SetUniform1f("material.diffuse", 1.f);
	_program->SetUniform1i("material.shininess", 1024);
	camera::Update();
	_program->SetUniform3fv("eyePos", glm::value_ptr(camera::GetCameraPos()));
	
	ecs::update();
	
	ImGui::SetNextWindowSize(ImVec2(_framebuf->get_width(), _framebuf->get_height()), ImGuiCond_Always);
	ImGui::Begin("window");
	
	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 pos = ImGui::GetWindowPos();

	width = size.x; height = size.y;
	
	_framebuf->set_size(width, height);

	ImGui::Image((ImTextureID)_framebuf->get_texture_2d("culori"), ImGui::GetContentRegionAvail() , ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	editor::draw();

	glfwSwapBuffers(_window);
	glfwPollEvents();


}

void application::Finalize()
{ 
	glDeleteProgram(_program->GetId());
	glfwTerminate();
}

