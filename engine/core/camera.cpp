#include "camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <unordered_map>

namespace camera
{

GLFWwindow* window;
glm::vec3 position;
glm::vec2 rotation;

glm::mat4 view_matrix;

bool W_DOWN = false;

std::unordered_map<int, bool> key_map;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ( action == GLFW_PRESS )
		key_map[key] = true;
	else if ( action == GLFW_RELEASE )
		key_map[key] = false;
}

void MouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	rotation.x = -xpos / 10;
	rotation.y = -ypos / 10;
}

void Initialize(GLFWwindow* win)
{
	window = win;

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCursorCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	position = glm::vec3(0.f, 0.f, 0.f);
	rotation = glm::vec2(0.f, 0.f);

	view_matrix = glm::mat4(1.f);
}

double camera_speed = 0.015;

void Update()
{
	glm::vec3 forward = glm::vec4(0.f, 0.f, -1.f, 1.f);
	glm::vec3 up = glm::vec4(0.f, 1.f, 0.f, 1.f);

	glm::mat4 transform = glm::mat4(1.f);
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 0.f));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 0.f));
	
	up = transform * glm::vec4(up, 1.f);
	forward = transform * glm::vec4(forward, 2.f);
	
	glm::vec3 difference = glm::vec3(0.f, 0.f, 0.f);

	if ( key_map[GLFW_KEY_W] )
		difference += forward;
	else if ( key_map[GLFW_KEY_S] )
		difference  -= forward;
	
	if ( key_map[GLFW_KEY_D] )
		difference -= glm::cross(up, forward);
	else if ( key_map[GLFW_KEY_A] )
		difference += glm::cross(up, forward);

	if ( key_map[GLFW_KEY_Z] )
		 difference += up;
	else if ( key_map[GLFW_KEY_X] )
		difference -= up;

	difference *= camera_speed;
	position += difference;
}

glm::mat4 GetViewMatrix()
{
	view_matrix = glm::mat4(1.f);
	view_matrix = glm::translate(view_matrix, position);
	view_matrix = glm::rotate(view_matrix, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 0.f));
	view_matrix = glm::rotate(view_matrix, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 0.f));
	return view_matrix;
}

}

