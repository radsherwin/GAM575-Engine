//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

// TEXTURE RENDER - Vertex Shader

#version 400 core

// Uniforms

	uniform mat4 view_matrix;
	uniform mat4 proj_matrix;
	uniform mat4 invBind[8];
	uniform mat4 boneWorld[8];   // world is inside

// Attributes
	layout (location = 0) in vec4 vert;    // x y z  (w=1)
	layout (location = 1) in vec3 norms;
	layout (location = 2) in vec2 uv;

	layout (location = 5) in vec4  weight;
	layout (location = 6) in ivec4 joint;

// Varying
	out VS_OUT
	{
		vec2 textCoordinate;
	} vs_out;


void main(void)
{
    vs_out.textCoordinate = uv;

	// Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!

	 mat4 skin_matrix = weight.x * boneWorld[joint.x] * invBind[joint.x]  +
					    weight.y * boneWorld[joint.y] * invBind[joint.y]  +
					    weight.z * boneWorld[joint.z] * invBind[joint.z]  +
					    weight.w * boneWorld[joint.w] * invBind[joint.w] ;
	
    gl_Position = proj_matrix * view_matrix * skin_matrix * vert;    
}

// --- End of File ---
