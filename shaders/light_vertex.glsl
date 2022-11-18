#version 330 core

layout (location=0) in vec3 vPos;
layout (location=1) in vec3 vNormal;

uniform vec3 lightPos;
uniform mat4 lightTransform;
uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projTransform;
uniform mat4 invModelTransform;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

void main(void)
{
	gl_Position = projTransform * viewTransform * modelTransform * vec4(vPos, 1.0);
	
	FragPos = vec3(modelTransform * vec4(vPos, 1.0));

	Normal = mat3(invModelTransform) * vNormal;

	LightPos = vec3(lightTransform * vec4(lightPos, 1.0));
}