// a simple colour vertex shader
#version 410

// position of the vertex
layout(location = 0) in vec4 Position;
// colour of the vertex
layout(location = 1) in vec4 Colour;

// passes the data onto the fragment shader
out vec4 vColour;

// used to move local-space vertices into clip space
uniform mat4 ProjectionViewModel;

void main()
{
	// stores the clip space position in the GLSL position constant
	gl_Position = ProjectionViewModel * Position;
	// outputs the given colour
	vColour = Colour;
}