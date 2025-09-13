#pragma once 
#include "core/common.h"

namespace data::framebuffer
{

class framebuffer;

framebuffer_id AddFramebuffer(int width, int height);
void BindFramebuffer(framebuffer_id id);
framebuffer* GetFramebuffer(framebuffer_id id);
void BindDefault();

}
