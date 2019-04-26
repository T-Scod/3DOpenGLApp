/*
	\file phong.frag
	\brief A normal map fragment shader
*/
#version 410

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
in vec4 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragTangent;
in vec3 fragBiTangent;

/*
	\def MAX_LIGHTS 10
	The maximum amount of lights.
	\var int numLights
	The number of lights.
	\struct Light
	\brief An object that emits light.
	\var vec4 position
	The position of the light.
	\var vec3 Ia
	The ambient colour of the light.
	\var vec3 Id
	The diffuse colour of the light.
	\var vec3 Is
	The specular colour of the light.
	\var float attenuation
	The reduction of the intensity of the light over distance.
	\var Light[] allLights
	All of the lights acting on the object.
*/
#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light
{
   vec4 position;
   vec3 Ia;
   vec3 Id;
   vec3 Is;
   float attenuation;
} allLights[MAX_LIGHTS];

/*
	\var vec3 Ka
	Ambient light from the material.
	\var vec3 Kd
	Diffuse light from the material.
	\var vec3 Ks
	Specular light from the material.
	\var float specularPower
	Specular power from the material.
*/
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

/*
	\var vec3 cameraPosition
	Position of the camera in world space
*/
uniform vec3 cameraPosition;

/*
	\var sampler2D diffuseTexture
	Used to sample the diffuse property from the texture.
	\var sampler2D specularTexture
	Used to sample the specular property from the texture.
	\var sampler2D normalTexture
	Gets the normal of the texture.
*/
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

/*
	\var vec4 finalColour
	The final colour of the pixel.
*/
out vec4 finalColour;

/*
	\fn vec3 ApplyLight(Light light, vec3 diffuseColor, vec3 specularColour, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera)
	\brief Calculates the colour to be added to the pixel colour using the properties of the lights in the scene.
	\param light The light that is being applied to the pixel.
	\param diffuseColor Diffuse colour component from the texture.
	\param specularColour Specular colour component from the texture.
	\param normal Normal component from the texture.
	\param surfacePos Position of the pixel.
	\param surfaceToCamera Vector from the pixel to the camera.
	\return Returns a colour that will be added to the pixel colour.
*/
vec3 ApplyLight(Light light, vec3 diffuseColor, vec3 specularColour, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera)
{
	// vector from the pixel to the light
    vec3 surfaceToLight;
	// coefficient of light intensity remaining over distance
    float attenuation = 1.0f;
	// checks if the light is directional
    if (light.position.w == 0.0f)
	{
        // the position of the light is used as the direction
        surfaceToLight = normalize(light.position.xyz);
		// no attenuation for directional lights
        attenuation = 1.0f;
    }
	// else, the light is a point light
	else
	{
        // the vector from the surface to the light source
        surfaceToLight = normalize(light.position.xyz - surfacePos);
		// distance between surface and light source
        float distanceToLight = length(light.position.xyz - surfacePos);
		// determines the remaining intensity of the light based on the distance
        attenuation = 1.0f / (1.0f + light.attenuation * pow(distanceToLight, 2));
    }

    // ambient component
    vec3 ambient = Ka * light.Ia;

	// brightness of the diffuse component
    float diffuseCoefficient = max(0.0f, dot(normal, surfaceToLight));
    // diffuse component
    vec3 diffuse = Kd * light.Id * diffuseCoefficient * diffuseColor.rgb;
    
    // brightness of the specular component
	float specularCoefficient = 0.0f;
    if(diffuseCoefficient > 0.0f)
	{
        specularCoefficient = pow(max(0.0f, dot(surfaceToCamera, reflect(surfaceToLight, normal))), specularPower);
	}
    // specular component
    vec3 specular = Ks * light.Is * specularCoefficient * specularColour;

    // linear color (color before gamma correction)
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
	// vector from the surface to the camera
	vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

    // combine color from all the lights
    vec3 linearColour = vec3(0);
    for (int i = 0; i < numLights; i++)
	{
		linearColour += ApplyLight(allLights[i], texDiffuse.rgb, texSpecular.rgb, normal, surfacePos, surfaceToCamera);
    }
    
    // final color (after gamma correction)
    vec3 gamma = vec3(1.0f / 2.2f);
	finalColour = vec4(pow(linearColour, gamma), texDiffuse.a);
}