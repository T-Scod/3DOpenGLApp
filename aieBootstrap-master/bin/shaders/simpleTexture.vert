// a texture vertex shader
#version 410

// position of the vertex
layout(location = 0) in vec4 Position;
// the corresponding coordinate on a texture
layout(location = 2) in vec2 TexCoord;

// passes the data onto the fragment shader
out vec2 vTexCoord;

// used to move local-space vertices into clip space
uniform mat4 ProjectionViewModel;

void main()
{
	// stores the clip space position in the GLSL position constant
	gl_Position = ProjectionViewModel * Position;
	// outputs the given texture coordinate
	vTexCoord = TexCoord;
}