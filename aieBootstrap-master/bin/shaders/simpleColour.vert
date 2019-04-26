/*
	\file simpleColour.vert
	\brief A simple colour vertex shader
*/
#version 410

/*
	\var vec4 vertPosition 
	Position of the vertex being passed in by the vertex array.
	\var vec4 vertColour
	Colour of the vertex being passed in by the vertex array.
*/
layout(location = 0) in vec4 vertPosition;
layout(location = 1) in vec4 vertColour;

/*
	\var vec4 fragColour
	Colour of the pixel that is passed to the fragment shader.
*/
out vec4 fragColour;

/*
	\var mat4 ProjectionViewModel
	Used to move local-space vertices into clip space.
*/
uniform mat4 ProjectionViewModel;

void main()
{
	// stores the clip space position in the GLSL position constant
	gl_Position = ProjectionViewModel * vertPosition;
	// outputs the given colour
	fragColour = vertColour;
}