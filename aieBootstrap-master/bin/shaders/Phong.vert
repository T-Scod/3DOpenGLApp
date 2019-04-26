/*
	\file phong.vert
	\brief A normal map vertex shader.
*/
#version 410

/*
	\var vec4 vertPosition
	Position of the vertex being passed in by the vertex array.
	\var vec4 vertNormal
	Normal of the vertex being passed in by the vertex array.
	\var vec2 vertTexCoord
	Texture coordinate of the vertex being passed in by the vertex array.
	\var vec4 vertTangent
	Tangent (along the x axis,) to the normal of the vertex being passed in by the vertex array.
*/
layout(location = 0) in vec4 vertPosition;
layout(location = 1) in vec4 vertNormal;
layout(location = 2) in vec2 vertTexCoord;
layout(location = 3) in vec4 vertTangent;

/*
	\var vec4 fragPosition
	Position of the vertex in world space.
	\var vec3 fragNormal
	Normal of the vertex in world space.
	\var vec2 fragTexCoord
	Texture coordinate of the vertex that is passed to the fragment shader.
	\var vec3 fragTangent
	Tangent of the vertex in world space.
	\var vec3 fragBiTangent
	Tangent (along the y axis,) to the normal of the vertex in world space.
*/
out vec4 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragTangent;
out vec3 fragBiTangent;

/*
	\var mat4 ProjectionViewModel
	Used to move local-space vertices into clip space.
	\var mat4 ModelMatrix
	Used to move local-space vertices into world space.
	\var mat3 NormalMatrix
	Used to transform the normal.
*/
uniform mat4 ProjectionViewModel;
uniform mat4 ModelMatrix;
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