#include "program.h" 
#include "common.h"
#include <iostream>

int program::GetUniformLocation(const std::string& uniform_name) 
{
	Bind();
	auto map_location = _uniform_cache.find(uniform_name);
	if ( map_location != _uniform_cache.end() )
		return map_location->second;

	int location = glGetUniformLocation(_id, uniform_name.c_str());

	assert(location != -1);

	_uniform_cache[uniform_name] = location;
	return location;
}

program::program()
{
	_id = glCreateProgram();
}

void program::AddShader(ShaderType type, const std::string& path)
{
	assert(_linked == false);
	GLenum internal_shader_type = GL_VERTEX_SHADER;

	switch (type) {
		case ShaderType::VERTEX:
		internal_shader_type = GL_VERTEX_SHADER;
		break;
 		case ShaderType::FRAGMENT:
		internal_shader_type = GL_FRAGMENT_SHADER;
		break;
		default:
		std::cout <<  "Error!" << std::endl;
		break;
	}
	_shaders.emplace_back(path, internal_shader_type);
}

void program::Link()
{
	assert(_linked == false);
	for ( auto& shader : _shaders )
		glAttachShader(_id, shader.GetId());

	glLinkProgram(_id);

	int success;
	char infoLog[512];
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if ( !success )
	{
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		std::cout << "Error linking program!" << std::endl;
		std::cout << infoLog << std::endl;
	}
	
	for ( auto& shader : _shaders )
	{
		glDetachShader(_id, shader.GetId());
		shader.Destroy();
	}
	_shaders.clear();
	_linked = true;

	assert(success);
}

void program::Bind()
{
	assert(_linked == true);
	assert( _id != 0 );
	glUseProgram(_id);
}
