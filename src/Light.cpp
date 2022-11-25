#include "stdafx.h"
#include "Light.h"
#include "Object.h"
#include "Shader.h"

Light::Light() : Object()
{
	mObject = new SharedObject(GetIdentitySphere());
	mObject->SetColor(ORANGE);
	mObject->SetPivot(&mPosition);

	glUseProgram(shd::GetShaderProgram(Shader::Light));
	shd::SetShader(Shader::Light, "light.color", glm::vec3(MyColor(WHITE)));
	shd::SetShader(Shader::Light, "light.intensity", 1.0f);
	shd::SetShader(Shader::Light, "light.ambient", 0.3f);
	shd::SetShader(Shader::Light, "light.diffuse", 1.0f);
	shd::SetShader(Shader::Light, "light.specular", 1.0f);
	shd::SetShader(Shader::Light, "light.shininess", 128.0f);
}

GLvoid Light::Draw() const
{
	//object->SetPosition(position);
	shd::SetShader(Shader::Light, "light.pos", mPosition);
	mObject->Draw();
}