#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 color;
layout(location=2) in vec2 texcoord;

out vec2 v_texcoord;

uniform mat4 u_mvp;

void main()
{
	v_texcoord = texcoord;
	gl_Position = u_mvp * vec4(position, 1.0);
}