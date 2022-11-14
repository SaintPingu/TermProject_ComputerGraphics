#include "stdafx.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"


glm::mat4 transform::GetWorld()
{
	glm::mat4 transform = glm::mat4(1.0f);

	return transform;
}

glm::mat4 transform::GetView(const Camera* camera)
{
	const glm::vec3 eye = camera->GetPosition();
	const glm::vec3 look = camera->GetLook();
	const glm::vec3 up = camera->GetUp();
	const glm::vec3 center = eye + look;

	glm::mat4 transform = glm::mat4(1.0f);
	transform *= glm::lookAt(eye, center, up);

	return transform;
}

extern glm::vec3 worldPosition;
glm::mat4 transform::GetProj(const Camera* camera)
{
	constexpr GLfloat farLength = 500;

	const glm::vec3 cameraPos = camera->GetPosition();
	const GLfloat size = glm::length(worldPosition - cameraPos) / 2;

	glm::mat4 transform = glm::mat4(1.0f);
	if (camera->IsPersp())
	{
		transform = glm::perspective(glm::radians(camera->GetFovY()), 1.0f, 0.1f, farLength);
	}
	else
	{
		transform = glm::ortho(-size, size, -size, size, -farLength, farLength);
	}

	return transform;
}



GLvoid transform::Apply(const glm::mat4& transform, const GLchar* name)
{
	const GLint shaderID = GetShaderProgram();
	const GLint location = glGetUniformLocation(shaderID, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));
}
GLvoid transform::Disable(const GLchar* name)
{
	const glm::mat4 transform = glm::mat4(1.0f);
	const GLint modelLocation = glGetUniformLocation(GetShaderProgram(), name);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transform));
}


GLvoid transform::ApplyColor(const glm::vec3& color)
{
	const GLchar* name = "newColor";
	const GLint shaderID = GetShaderProgram();
	GLint location = glGetUniformLocation(shaderID, name);
	glUniform3fv(location, 1, glm::value_ptr(color));

	location = glGetUniformLocation(shaderID, "isChangeColor");
	glUniform1i(location, true);
}
GLvoid transform::DisableColor()
{
	const GLint shaderID = GetShaderProgram();
	const GLint location = glGetUniformLocation(shaderID, "isChangeColor");
	glUniform1i(location, false);
}