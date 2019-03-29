// a simple flat colour shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 lightDirection;

uniform vec3 cameraPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

out vec4 FragColour;

void main()
{
	vec3 N = normalize(vNormal);
	vec3 L = normalize(lightDirection);

	float lambertTerm = max(0, min(1, dot(N, -L)));

	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L, N);

	float specularTerm = pow(max(0, dot(R, V)), specularPower);

	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * texture(diffuseTexture, vTexCoord).rgb * lambertTerm;
	vec3 specular = Is * Ks * texture(specularTexture, vTexCoord).rgb * specularTerm;

	FragColour = vec4(ambient + diffuse + specular, 1);
}