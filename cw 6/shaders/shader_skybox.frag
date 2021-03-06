#version 410 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

float near = 0.1f;
float far = 70.0f;

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth)
{
	// these 2 values were default function 
	// parameters before & didn't always work
	float steepness = 0.4f;
	float offset = 50.0f; //50.0f

	float zVal = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zVal - offset))));
}


void main()
{   
	float depth = logisticDepth(gl_FragCoord.z);
    FragColor = texture(skybox, TexCoords) * (1.0f - depth) + vec4(depth * vec3(5.0f/255.0f, 70.0f/255.0f, 80.0f/255.0f), 1.0f);
	//FragColor = texture(skybox, TexCoords) * (1.0f - depth) + vec4(depth * vec3(102.0f/255.0f, 172.0f/255.0f, 233.0f/255.0f), 1.0f);
	//FragColor = texture(skybox, TexCoords);
}