#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
out vec4 color;
out vec2 texCoord;
void main()
{
	texCoord = aTexCoord;
	color = vec4(aColor, 1.f);
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
