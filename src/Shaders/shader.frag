#version 330

// FRAGMENT SHADER

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3; // Has to be the same value as in the CommonValues.h file.
const int MAX_SPOT_LIGHTS = 3; // Has to be the same value as in the CommonValues.h file.

struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base; // Somewhat inheritance of the Light struct.
	vec3 direction;
};

struct PointLight 
{
	Light base; // Somewhat inheritance of the Light struct.
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base; // Somewhat inheritance of the PointLight struct.
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

// Position of the eye, of the camera
uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction)
{
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

	// Calculating diffuse lighting
	
	// Calculating the value between 0 and 1 that gives how much diffuse light there is, as in how dark the surface gets.
	// We normalize so that the dot product ONLY takes into account the cos of the angle between the vectors. Normalizing removes the magnitude of each vector from the equation.
	// And we want no negative values. At minimum, the factor can be 0 (dark).
	float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
	
	vec4 diffuseColor = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);
	
	// Initialize a specular color as "null". If used at this point, will do nothing.
	vec4 specularColour = vec4(0, 0, 0, 0);
	
	// If we have diffuse lighting... Then we apply specular lighting. Can't have specular without diffuse.
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - fragPos);
		// Vector of where the light is reflected around the normal. We want light to bounce off on the other side of the normal.
		// Reflect does this. First argument is what we want to reflect, and the second is what we are reflecting around.
		vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));
		
		// Now we find the angle between those 2 vectors. This is why we normalized both of those vectors, so we can get the cos between the two.
		float specularFactor = dot(fragToEye, reflectedVertex);
		
		if(specularFactor > 0.0f)
		{
			// Pow applies a power to a number.
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	return (ambientColour + diffuseColor + specularColour);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight(PointLight pLight)
{
	// Getting vector from point light to fragment.
	vec3 direction = fragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	vec4 colour = CalcLightByDirection(pLight.base, direction);

	// Formula to calculate attenuation. See theory.
	float attenuation = pLight.exponent * distance * distance +
	pLight.linear * distance +
	pLight.constant;

	return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(fragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);
	
	if(slFactor > sLight.edge)
	{
		vec4 colour = CalcPointLight(sLight.base);
		return (colour);// * (1.0f - (1.0f - slFactor) * 1.0f / (1.0f - SpotLight.edge)));
	}
	else
	{
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);

	// Loop over point lights and add them to total colour.
	for(int i =0; i < spotLightCount; i++)
	{
		totalColour += CalcSpotLight(spotLights[i]);
	}

	return totalColour;
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	
	// Loop over point lights and add them to total colour.
	for(int i =0; i < pointLightCount; i++)
	{
		totalColour += CalcPointLight(pointLights[i]);
	}
	
	return totalColour;
}

void main()
{	
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();
	
	colour = texture(theTexture, texCoord) * finalColour;
}