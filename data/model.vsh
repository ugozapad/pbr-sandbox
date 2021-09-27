#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

varying vec3 v_worldPos;
varying vec3 v_normal;
varying vec2 v_texcoord;

uniform mat4 u_model;
uniform mat3 u_normalMatrix;
uniform mat4 u_mvp;

void main()
{
	v_worldPos = vec4(u_model * vec4(position, 1.0)).xyz;
	v_normal = u_normalMatrix * normal;
	v_texcoord = texcoord;
	
	gl_Position = u_mvp * vec4(position, 1.0);
}