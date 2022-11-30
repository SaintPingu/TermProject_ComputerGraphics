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
	vec3 ambient = light.ambient * light.color;

	vec3 normalVector = normalize(Normal);
	vec3 lightDir = normalize(light.pos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);
	vec3 diffuse = diffuseLight * light.color * light.diffuse;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalVector);
	float specularLight = max(dot(viewDir, reflectDir), 0.0);
	specularLight = pow(specularLight, light.shininess);
	vec3 specular = specularLight * light.color * light.specular;

	vec3 result = (ambient + diffuse + specular) * light.intensity;

	FragColor = texture(outTexture, TexCoord) * vec4(result, 0.5);
}