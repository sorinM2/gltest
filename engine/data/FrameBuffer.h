#pragma once
#include "core/GLCommon.h"
#include "utility/vector.h"
#include <unordered_map>

namespace data 
{

class FrameBuffer 
{
public:
	struct FrameBufferBuffer
	{	
		GLenum _format;
		GLenum _attachment;
		unsigned int _id{id::invalid_id};
	};

	FrameBuffer(int width, int height, bool fixed_ratio = false);	
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


	void set_size(int width, int height);
	
	void destroy();
private:
	void update_textures();
	void update_renderbuffers();

private:
	unsigned int _id{id::invalid_id};
	std::unordered_map<std::string, FrameBufferBuffer> _render_buffers;
	std::unordered_map<std::string, FrameBufferBuffer> _textures_2d;
	int _width, _height;
	float _aspect_ratio = 0.f;

	glm::vec3 _clear_color = {0.2f, 0.2f, 0.2f};
	
};

}
