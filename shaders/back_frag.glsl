#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 objectColor;
uniform sampler2D outTexture;

void main(void)
{
	vec4 TexColor = texture(outTexture, TexCoord) * vec4(objectColor, 1.0f);
	if (TexColor.a < 0.5)
		discard;

	FragColor = TexColor;
}