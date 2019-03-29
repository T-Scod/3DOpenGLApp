// a simple flat colour shader
#version 410

in vec4 vColour;
in vec2 vTexCoord;

uniform sampler2D diffuseTexture;

out vec4 FragColour;

void main()
{
	FragColour = vColour;
}