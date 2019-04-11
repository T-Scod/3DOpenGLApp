// a simple lighting fragment shader
#version 410

// outputs from the vertex shader
in vec4 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

uniform struct Light
{
	vec3 position;
	vec3 intensities;
	float attenuation;
	vec3 ambientCoefficient;
} light;

uniform sampler2D materialTex;
uniform vec3 cameraPosition;
uniform float materialShininess;
uniform vec3 materialSpecularColour;

// the colour of the pixel
out vec4 finalColour;

void main()
{
	vec3 normal = normalize(fragNormal);
	vec4 surfaceColour = texture(materialTex, fragTexCoord);
	vec3 surfaceToLight = normalize(light.position - vec3(fragPosition));
	vec3 surfaceToCamera = normalize(cameraPosition - vec3(fragPosition));

	vec3 ambient = light.ambientCoefficient * surfaceColour.rgb * light.intensities;
	
	float diffuseCoefficient = max(0.0f, dot(normal, surfaceToLight));
	vec3 diffuse = diffuseCoefficient * surfaceColour.rgb * light.intensities;
	
	float specularCoefficient = 0.0f;
	if (diffuseCoefficient > 0.0f)
	{
		specularCoefficient = pow(max(0.0f, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
	}
	vec3 specular = specularCoefficient * materialSpecularColour * light.intensities;

	float distanceToLight = length(light.position - vec3(fragPosition));
	float attenuation = 1.0f / (1.0f + light.attenuation * pow(distanceToLight, 2));

	vec3 linearColour = ambient + attenuation * (diffuse + specular);
	vec3 gamma = vec3(1.0f/2.2f);

	finalColour = vec4(pow(linearColour, gamma), surfaceColour.a);
}