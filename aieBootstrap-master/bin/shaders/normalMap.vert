// a normal map vertex shader
#version 410

// position of the vertex
layout(location = 0) in vec4 Position;
// normal of the vertex (used for lighting)
layout(location = 1) in vec4 Normal;
// the corresponding coordinate on a texture
layout(location = 2) in vec2 TexCoord;
// tangent to the normal (points in the direction of the X axis)
layout(location = 3) in vec4 Tangent;

// passes the data onto the fragment shader
out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vTangent;
// points in the Y direction of the texture
out vec3 vBiTangent;

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
	// transforms the tangent
	vTangent = NormalMatrix * Tangent.xyz;
	// gets the BiTangent from the cross product between the normal and tangent (Y = cross(Z, X))
	vBiTangent = cross(vNormal, vTangent) * Tangent.w;
}