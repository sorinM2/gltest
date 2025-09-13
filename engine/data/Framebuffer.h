#pragma once
#include "core/GLCommon.h"
#include "utility/vector.h"
#include <unordered_map>

namespace data::framebuffer
{

class framebuffer 
{
public:
	struct FramebufferBuffer
	{	
		GLenum _format;
		GLenum _attachment;
		unsigned int _id{id::invalid_id};
	};

	framebuffer(int width, int height);	
	void clear();

	void add_renderbuffer(const std::string& name, GLenum format, GLenum attachment);
	void add_texture_2d(const std::string& name, GLenum format, GLenum attachment);

	unsigned int get_texture_2d(const std::string& name);
	unsigned int get_renderbuffer(const std::string& name);

	void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, _id);}

	void bind_texture(const std::string& name);

	unsigned int get_id() const { return _id; }
	int get_width() const { return _width; }
	int get_height() const { return  _height; }
	float get_aspect_ratio() const { return (float)_width / (float)_height; }

	void set_size(int width, int height);
	
	void destroy();
private:
	void update_textures();
	void update_renderbuffers();

private:
	unsigned int _id{id::invalid_id};
	std::unordered_map<std::string, FramebufferBuffer> _render_buffers;
	std::unordered_map<std::string, FramebufferBuffer> _textures_2d;
	int _width, _height;

	glm::vec3 _clear_color = {0.2f, 0.2f, 0.2f};
	
};

}
