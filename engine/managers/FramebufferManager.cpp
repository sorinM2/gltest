#include "FramebufferManager.h"
#include "utility/vector.h"
#include "data/Framebuffer.h"

namespace data::framebuffer 
{
utl::vector<framebuffer, false> framebuffers;

unsigned int AddFramebuffer(int width, int height)
{
	return framebuffers.emplace_tombstone(width, height);
}

void BindFramebuffer(framebuffer_id id)
{
	assert(id < framebuffers.size());
	framebuffers[id].bind();
}

void BindDefault()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

framebuffer* GetFramebuffer(unsigned int id)
{
	assert(id < framebuffers.size());
	return &framebuffers[id];
}
}
