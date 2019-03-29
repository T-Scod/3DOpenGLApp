// a simple colour fragment shader
#version 410

// outputs from the vertex shader
in vec4 vColour;

// the colour of the pixel
out vec4 FragColour;

void main()
{
	// sets the pixel colour to the given colour
	FragColour = vColour;
}