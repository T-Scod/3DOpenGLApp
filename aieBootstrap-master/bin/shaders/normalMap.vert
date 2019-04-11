// a normal map vertex shader
#version 410

// position of the vertex
layout(location = 0) in vec4 vertPosition;
// normal of the vertex (used for lighting)
layout(location = 1) in vec4 vertNormal;
// the corresponding coordinate on a texture
layout(location = 2) in vec2 vertTexCoord;
// tangent to the normal (points in the direction of the X axis)
layout(location = 3) in vec4 vertTangent;

// passes the data onto the fragment shader
out vec4 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragTangent;
// points in the Y direction of the texture
out vec3 fragBiTangent;

// used to move local-space vertices into clip space
uniform mat4 ProjectionViewModel;
// used to move local-space vertices into world space
uniform mat4 ModelMatrix;
// used to transform the normal
uniform mat3 NormalMatrix;

void main()
{
	// stores the clip space position in the GLSL position constant
	gl_Position = ProjectionViewModel * vertPosition;
	// transforms the position into world space before passing it to the fragment shader
	fragPosition = ModelMatrix * vertPosition;
	// transforms the normal
	fragNormal = NormalMatrix * vertNormal.xyz;
	// outputs the given texture coordinate
	fragTexCoord = vertTexCoord;
	// transforms the tangent
	fragTangent = NormalMatrix * vertTangent.xyz;
	// gets the BiTangent from the cross product between the normal and tangent (Y = cross(Z, X))
	fragBiTangent = cross(fragNormal, fragTangent) * vertTangent.w;
}