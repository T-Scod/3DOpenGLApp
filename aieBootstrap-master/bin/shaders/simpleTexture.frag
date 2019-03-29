// a texture fragment shader
#version 410

// outputs from the vertex shader
in vec2 vTexCoord;

// used to sample the diffuse property from the texture
uniform sampler2D diffuseTexture;

// the colour of the pixel
out vec4 FragColour;

void main()
{
	// sets the pixel colour to the colour of the texture at the given coordinate
	FragColour = texture( diffuseTexture, vTexCoord );
}