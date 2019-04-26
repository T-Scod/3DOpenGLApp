/*
	\file simpleColour.frag
	\brief A simple colour fragment shader.
*/
#version 410

/*
	\var vec4 fragColour
	Colour of the pixel that is passed in from the vertex shader.
*/
in vec4 fragColour;

/*
	\var vec4 finalColour
	The final colour of the pixel.
*/
out vec4 finalColour;

void main()
{
	// sets the pixel colour to the given colour
	finalColour = fragColour;
}