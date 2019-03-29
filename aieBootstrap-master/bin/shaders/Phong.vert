// a simple lighting vertex shader
#version 410

// position of the vertex
layout(location = 0) in vec4 Position;
// normal of the vertex (used for lighting)
layout(location = 1) in vec4 Normal;
// the corresponding coordinate on a texture
layout(location = 2) in vec2 TexCoord;

// passes the data onto the fragment shader
out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

// used to move local-space vertices into clip space
uniform mat4 ProjectionViewModel;
// used to move local-space vertices into world space
uniform mat4 ModelMatrix;
// used to transform the normal
uniform mat3 NormalMatrix;

void main()
{
	// stores the clip space position in the GLSL position constant
	gl_Position = ProjectionViewModel * Position;
	// transforms the position into world space before passing it to the fragment shader
	vPosition = ModelMatrix * Position;
	// transforms the normal
	vNormal = NormalMatrix * Normal.xyz;
	// outputs the given texture coordinate
	vTexCoord = TexCoord;
}