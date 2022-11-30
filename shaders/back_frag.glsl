#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform mat4 lightTransform;

uniform sampler2D outTexture;

struct Light {
	vec3 pos;
	vec3 color;

	// Intensity
	float intensity;	// global intensity
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

uniform Light light;

void main(void)
{
	FragColor = texture(outTexture, TexCoord);
}