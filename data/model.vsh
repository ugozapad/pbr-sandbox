#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

out vec3 v_fragPos;
out vec3 v_normal; // translated model normal
out vec2 v_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_mvp;

void main()
{
	v_fragPos = vec3( u_model * vec4(position, 1.0) );
	v_normal = normal;
	v_texcoord = texcoord;
	
	gl_Position = u_mvp * vec4(position, 1.0);
}