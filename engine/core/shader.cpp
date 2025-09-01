#include "shader.h" 
#include <fstream>
#include <sstream>
#include "common.h"
#include <iostream>
#include "utility/unmove.h"

shader::shader(const std::string& path, GLenum shader_type)
{
	_path = path;
	std::ifstream shader(path);
	{
		std::stringstream buffer;
		buffer << shader.rdbuf();

		//std:: cout << "shader Shader code:\n" << buffer.str() << std::endl;

		_id = glCreateShader(shader_type);
		
		glShaderSource(_id, 1, &util::unmove(buffer.str().c_str()), NULL);
		glCompileShader(_id);
	}

	int success;
	char infoLog[512];
	glGetShaderiv(_id, GL_COMPILE_STATUS, &success);

	if ( !success )
	{
		glGetShaderInfoLog(_id, 512, NULL, infoLog);
		std::cout << "Error compilating: " << path << std::endl;
		std::cout << infoLog << std::endl;
	}
	shader.close();

}

void shader::Destroy()
{
	_active = false;
	glDeleteShader(_id);
}
