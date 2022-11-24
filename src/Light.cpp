#include "stdafx.h"
#include "Light.h"
#include "Object.h"
#include "Shader.h"

Light::Light() : Object()
{
	object = new SharedObject(GetIdentitySphere());
	object->SetColor(ORANGE);

	glUseProgram(GetShaderProgram(Shader::Light));
	ApplyLightColorRef(WHITE);
	::SetShader(Shader::Light, "light.ambient", 0.3f);
	::SetShader(Shader::Light, "light.diffuse", 1.0f);
	::SetShader(Shader::Light, "light.specular", 1.0f);
	::SetShader(Shader::Light, "light.shininess", 128.0f);
}

GLvoid Light::Draw() const
{
	//object->SetPosition(position);
	::SetShader(Shader::Light, "light.pos", position);
	object->Draw();
}