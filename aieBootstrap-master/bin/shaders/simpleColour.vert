// a simple shader
#version 410

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Colour;
layout( location = 2 ) in vec2 TexCoord;

out vec4 vColour;
out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;

void main()
{
	gl_Position = ProjectionViewModel * Position;
	vColour = Colour;
	vTexCoord = TexCoord;
}