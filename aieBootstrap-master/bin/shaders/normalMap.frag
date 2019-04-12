// a normal map fragment shader
#version 410

// outputs from the vertex shader
in vec4 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragTangent;
in vec3 fragBiTangent;

#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light {
   vec4 position;
   //a.k.a the color of the light
   vec3 intensities;
   float attenuation;
   float ambientCoefficient;
   float coneAngle;
   vec3 coneDirection;
} allLights[MAX_LIGHTS];

// material ambient
uniform vec3 Ka;
// material diffuse
uniform vec3 Kd;
// material specular
uniform vec3 Ks;
// material specular power
uniform float specularPower;

// position of the camera in world space
uniform vec3 cameraPosition;

// used to sample the diffuse property from the texture
uniform sampler2D diffuseTexture;
// used to sample the specular property from the texture
uniform sampler2D specularTexture;
// gets the normal of the texture
uniform sampler2D normalTexture;

// the colour of the pixel
out vec4 finalColour;

vec3 ApplyLight(Light light, vec3 diffuseColor, vec3 specularColour, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera)
{
    vec3 surfaceToLight;
    float attenuation = 1.0f;
    if (light.position.w == 0.0f)
	{
        //directional light
        surfaceToLight = normalize(light.position.xyz);
		//no attenuation for directional lights
        attenuation = 1.0f;
    }
	else
	{
        //point light
        surfaceToLight = normalize(light.position.xyz - surfacePos);
        float distanceToLight = length(light.position.xyz - surfacePos);
        attenuation = 1.0f / (1.0f + light.attenuation * pow(distanceToLight, 2));

        //cone restrictions (affects attenuation)
        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
        if(lightToSurfaceAngle > light.coneAngle)
		{
            attenuation = 0.0f;
        }
    }

    //ambient
    vec3 ambient = Ka * light.ambientCoefficient * diffuseColor.rgb * light.intensities;

    //diffuse
    float diffuseCoefficient = max(0.0f, dot(normal, surfaceToLight));
    vec3 diffuse = Kd * diffuseCoefficient * diffuseColor.rgb * light.intensities;
    
    //specular
    float specularCoefficient = 0.0f;
    if(diffuseCoefficient > 0.0f)
	{
        specularCoefficient = pow(max(0.0f, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), specularPower);
	}
    vec3 specular = Ks * specularCoefficient * specularColour * light.intensities;

    //linear color (color before gamma correction)
    return ambient + attenuation * (diffuse + specular);
}

void main()
{
	// ensures that the vectors are normalised
	vec3 normal = normalize(fragNormal);
	vec3 tangent = normalize(fragTangent);
	vec3 biTangent = normalize(fragBiTangent);
	// tangent basis matrix
	mat3 TBN = mat3(tangent, biTangent, normal);

	// gets the property at the specified coordinate
	// surface colour
	vec4 texDiffuse = texture(diffuseTexture, fragTexCoord).rgba;
	// material specular colour
	vec3 texSpecular = texture(specularTexture, fragTexCoord).rgb;
	vec3 texNormal = texture(normalTexture, fragTexCoord).rgb;

	// transforms the normal out of a 0 to 1 range into a -1 to 1 range
	normal = TBN * (texNormal * 2 - 1);
	vec3 surfacePos = vec3(fragPosition);
	vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

	vec3 linearColour = vec3(0);
	for (int i = 0; i < numLights; i++)
	{
		linearColour += ApplyLight(allLights[i], texDiffuse.rgb, texSpecular.rgb, normal, surfacePos, surfaceToCamera);
	}

	// final colour after gamma correction
	vec3 gamma = vec3(1.0f / 2.2f);
	finalColour = vec4(pow(linearColour, gamma), texDiffuse.a);
}