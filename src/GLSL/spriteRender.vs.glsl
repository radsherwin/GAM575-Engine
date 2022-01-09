#version 400 core

//Inputs for VertexShader
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tc;

uniform mat4 world_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform mat4 uv_matrix;

//values that Vertex Shader outputs
out VS_OUT
{
	vec2 textCoordinate;
	mat4 uv_mat;
} vs_out;

void main(void)
{
	//Squirrel away the texture and the uv matrix to the fragment stage
	vs_out.textCoordinate = tc;
	vs_out.uv_mat = uv_matrix;

	gl_Position = proj_matrix * view_matrix * world_matrix * position;
}