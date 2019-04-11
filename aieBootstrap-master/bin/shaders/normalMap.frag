// a normal map fragment shader
#version 410

// outputs from the vertex shader
in vec4 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragTangent;
in vec3 fragBiTangent;

#define MAX_LIGHTS 10
uniform struct Light
{
	vec3 position;
	vec3 intensities;
} allLights[MAX_LIGHTS];

// material ambient
uniform vec3 Ka;
// material diffuse
uniform vec3 Kd;
// material specular
uniform vec3 Ks;
// material specular power
uniform float specularPower;
// light ambient
uniform vec3 Ia;
// light diffuse
uniform vec3 Id;
// light specular
uniform vec3 Is;
// direction of the light
uniform vec3 lightDirection;

// position of the camera in world space
uniform vec3 cameraPosition;

// used to sample the diffuse property from the texture
uniform sampler2D diffuseTexture;
// used to sample the specular property from the texture
uniform sampler2D specularTexture;
// gets the normal of the texture
uniform sampler2D normalTexture;

// the colour of the pixel
out vec4 FragColour;

void main()
{
	// ensures that the vectors are normalised
	vec3 N = normalize(fragNormal);
	vec3 T = normalize(fragTangent);
	vec3 B = normalize(fragBiTangent);
	// tangent basis matrix
	mat3 TBN = mat3(T, B, N);

	// gets the property at the specified coordinate
	vec3 texDiffuse = texture(diffuseTexture, fragTexCoord).rgb;
	vec3 texSpecular = texture(specularTexture, fragTexCoord).rgb;
	vec3 texNormal = texture(normalTexture, fragTexCoord).rgb;

	// transforms the normal out of a 0 to 1 range into a -1 to 1 range
	N = TBN * (texNormal * 2 - 1);
	// normalised light direction
	vec3 L = normalize(lightDirection);
	// clamps the lambert scalar between 0 and 1
	float lambertTerm = max(0, min(1, dot(N, -L)));

	// view vector
	vec3 V = normalize(cameraPosition - fragPosition.xyz);
	// reflection vector
	vec3 R = reflect(L, N);
	// calculates specular term
	float specularTerm = pow(max(0, dot(R, V)), specularPower);

	// calculate each light property
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * texDiffuse * lambertTerm;
	vec3 specular = Is * Ks * texSpecular * specularTerm;

	// combines the light properties and sends out the fragment colour
	FragColour = vec4(ambient + diffuse + specular, 1);
}