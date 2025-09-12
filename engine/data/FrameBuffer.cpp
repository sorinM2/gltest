#include "FrameBuffer.h"

namespace data 
{

FrameBuffer::FrameBuffer(int width, int height, bool fixed_ratio) : _width(width), _height(height)	
{
	if ( fixed_ratio )
		_aspect_ratio = (float)width / (float)height;
	glGenFramebuffers(1, &_id);
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	glViewport(0, 0, width, height);
}
	

void FrameBuffer::add_renderbuffer(const std::string& name, GLenum format, GLenum attachment)
{
	if ( _render_buffers.find(name) != _render_buffers.end() )
	{
		spdlog::error("Renderbuffer with name {0} already exists!", name);
		return;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, _id);	

	_render_buffers[name]._format = format;
	_render_buffers[name]._attachment = attachment;

	glGenRenderbuffers(1, &_render_buffers[name]._id);

	glBindRenderbuffer(GL_RENDERBUFFER, _render_buffers[name]._id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);	

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, _render_buffers[name]._id);
}

void FrameBuffer::update_renderbuffers()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _id);	

	for ( auto buffer : _render_buffers )
	{
		glBindRenderbuffer(GL_RENDERBUFFER, buffer.second._id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);	

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, buffer.second._attachment, GL_RENDERBUFFER, buffer.second._id);
	}
}

void FrameBuffer::add_texture_2d(const std::string& name, GLenum format, GLenum attachment)
{
	if ( _textures_2d.find(name) != _textures_2d.end() )
	{
		spdlog::error( "Framebuffer texture with name {0} already exists!", name);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _id);	

	_textures_2d[name]._format = format;
	_textures_2d[name]._attachment = attachment;

	glGenTextures(1, &_textures_2d[name]._id);
	glBindTexture(GL_TEXTURE_2D, _textures_2d[name]._id);

	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, NULL);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, _textures_2d[name]._id, 0);
}

void FrameBuffer::update_textures()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _id);	

	for ( auto& texture : _textures_2d )
	{
		glBindTexture(GL_TEXTURE_2D, texture.second._id);

		glTexImage2D(GL_TEXTURE_2D, 0, texture.second._format, _width, _height, 0, texture.second._format, GL_UNSIGNED_BYTE, NULL);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glFramebufferTexture2D(GL_FRAMEBUFFER, texture.second._attachment, GL_TEXTURE_2D, texture.second._id, 0);
	}

}

unsigned int FrameBuffer::get_texture_2d(const std::string& name)
{
	if ( _textures_2d.find(name) == _textures_2d.end() )
	{
		spdlog::error( "Framebuffer texture with name {0} doesn't exists!", name);
		return 0;
	}

	return _textures_2d[name]._id;
}

unsigned int FrameBuffer::get_renderbuffer(const std::string& name) 
{
	if ( _render_buffers.find(name) == _render_buffers.end() )
	{
		spdlog::error( "Framebuffer renderbuffer with name {0} doesn't exists!", name);
		return 0;
	}

	return _render_buffers[name]._id;
}


void FrameBuffer::bind_texture(const std::string& name)
{
	if ( _textures_2d.find(name) == _textures_2d.end() )
	{
		spdlog::error( "Framebuffer texture with name {0} doesn't exists!", name);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, _textures_2d[name]._id);
}

void FrameBuffer::set_size(int width, int height)
{
	if ( _aspect_ratio != 0.f )
	{
		if ( (float)width / (float)height > _aspect_ratio ) 
			width = height * _aspect_ratio;
		else height = width / _aspect_ratio;
	}
	

	if ( width == _width or height == _height)
		return; 

	_width = width;
	_height = height;

	update_renderbuffers();
	update_textures();
	clear();
	glViewport(0, 0, width, height);
}

void FrameBuffer::clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _id);	
	glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::destroy()
{
	for ( auto& texture : _textures_2d )
		glDeleteTextures(1, &texture.second._id );

	for ( auto& renderbuffer : _render_buffers )
		glDeleteRenderbuffers(1, &renderbuffer.second._id );

	glDeleteFramebuffers(1, &_id);
}


}
