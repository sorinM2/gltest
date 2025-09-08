#include "TextureManager.h"
#include <array>
#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"
#include <unordered_map>
#include "assets/AssetsPath.h"
#include <filesystem>

namespace textures 
{

std::array<GLenum, 32> gl_texture_slots = 
{
	GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4,
	GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8, GL_TEXTURE9, 
	GL_TEXTURE10, GL_TEXTURE11, GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14,
	GL_TEXTURE15, GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19, 
	GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23, GL_TEXTURE24,
	GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29,
	GL_TEXTURE30, GL_TEXTURE31
};

void normalize_path(std::string& path)
{
	auto p = std::filesystem::canonical(path);
	path = p.string();
}

std::unordered_map<std::string, texture_2d> textures;
utl::vector<int, false> free_slots;

void add_texture(std::string path, GLenum internal_format, GLenum format)
{
	normalize_path(path);

	if ( textures.find(path) != textures.end() )
	{
		spdlog::warn( "Warning! Texture with path: {0} already exists!", path);
		return;
	}
	
	texture_2d new_tex;

	textures[path] = new_tex;
	textures[path].initialize(path, internal_format, format);
}

unsigned int bind_texture(std::string path)
{
	normalize_path(path);
	
	auto iter = textures.find(path);
	if ( iter == textures.end() )
	{
		spdlog::error( "Error! Texture with path: {0} doesn't exists!", path);
		return ~(0);
	}


	texture_2d* texture = &iter->second;

	unsigned int slot = free_slots.emplace_tombstone();

	assert( slot < gl_texture_slots.size() );

	if ( slot >= gl_texture_slots.size() )
	{
		spdlog::error( " Error! Too many texture binded at once!");
		return ~(0);
	}
	texture->bind(slot);	
	
	return slot;
}

void unbind_all()
{
	for ( auto& c : textures )
		c.second.unbind();
	free_slots.clear();
}

utl::vector<unsigned int> set_texture_list(utl::vector<std::string> list)
{
	unbind_all();
	utl::vector<unsigned int> slots;
	assert(list.size() <= gl_texture_slots.size());

	for ( auto& texture_path : list )
	{
		unsigned int slot = bind_texture(texture_path);	
		slots.emplace_back(slot);
	}

	return slots;
}

void texture_2d::initialize(std::string path, GLenum internal_format, GLenum format)
{
	glGenTextures(1, &_id);
	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;

	normalize_path(path);	

	_path = path;


	unsigned char* data = stbi_load((path).c_str(), &width, &height, &channels, 0);
	
	assert(data != nullptr);
	if ( data == nullptr ){
		spdlog::error( "Error! Failed to create texture with path: {0}", path);
		glDeleteTextures(1, &_id);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, _id);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void texture_2d::bind(unsigned int slot)
{
	glActiveTexture(gl_texture_slots[slot]);
	glBindTexture(GL_TEXTURE_2D, _id);
	_slot = slot;
}

void texture_2d::unbind()
{
	_slot = ~(0);
}

}
