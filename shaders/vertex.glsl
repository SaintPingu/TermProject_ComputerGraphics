#version 330 core

layout (location=0) in vec3 vPos;
layout (location=1) in vec3 objectColor;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projTransform;

uniform bool isChangeColor;
uniform vec3 newColor;

out vec3 out_Color;

void main(void)
{
	gl_Position = projTransform * viewTransform * modelTransform * vec4(vPos, 1.0);

	out_Color = objectColor;
}