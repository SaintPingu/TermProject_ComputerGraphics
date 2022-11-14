#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(const glm::vec3& position)
{
	this->SetPosition(position);
}

GLvoid Camera::SetPosition(const glm::vec3& position)
{
	this->position = position;
}


GLvoid Camera::Move(const glm::vec3& vector)
{
	Camera::MoveX(vector.x);
	Camera::MoveX(vector.y);
	Camera::MoveX(vector.z);
}
GLvoid Camera::MoveX(const GLfloat& amount)
{
	glm::vec3 right = GetRight();
	this->position += -right * amount * velocity;
}
GLvoid Camera::MoveY(const GLfloat& amount)
{
	glm::vec3 up = GetUp();
	this->position += up * amount * velocity;
}
GLvoid Camera::MoveZ(const GLfloat& amount)
{
	glm::vec3 look = GetLook();
	this->position += look * amount * velocity;
}
GLvoid Camera::MoveGlobal(const glm::vec3& offset)
{
	position += offset * velocity;
}

GLvoid Camera::Look(const glm::vec3& point)
{
	if (pivot == nullptr)
	{
		this->look = glm::normalize(point - position);
	}
	else
	{
		this->look = glm::normalize(point - (position + *pivot));
	}
}
GLvoid Camera::SetLook(const glm::vec3& look)
{
	this->look = look;
}
GLvoid Camera::RotateAxis(const GLfloat& theta, const glm::vec3& axis)
{
	Rotate(look, theta, axis);
}

GLvoid Camera::RotateLocal(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll)
{
	if (yaw != 0.0f)
	{
		Rotate(look, glm::radians(yaw), GetRight());
	}
	if (pitch != 0.0f)
	{
		Rotate(look, glm::radians(pitch), GetUp());
	}
	if (roll != 0.0f)
	{
		Rotate(look, glm::radians(roll), GetLook());
	}
}

extern glm::vec3 worldPosition;
GLvoid Camera::RotateYAxis(const GLfloat& theta)
{
	GLfloat radius = glm::length(position - worldPosition);
	glm::vec3 nextPosition = Vector3::RotateOrigin(position, theta, Vector3::Up());
	position = nextPosition;

	glm::vec3 target = glm::normalize(worldPosition - position);
	look.x = target.x;
	look.z = target.z;
}
GLvoid Camera::RotatePosition(const glm::vec3& axis, const GLfloat& degree)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(degree), glm::normalize(axis));
	position -= *pivot;
	MultiplyVector(transform, position);
	position += *pivot;
}

GLvoid Camera::SetPivot(const glm::vec3* pivot)
{
	this->pivot = pivot;
}
glm::vec3 Camera::GetPosition() const
{
	if (pivot != nullptr)
	{
		return position + *pivot;
	}
	else
	{
		return position;
	}
}
glm::vec3 Camera::GetLook() const
{
	return look;
}
glm::vec3 Camera::GetRight() const
{
	return glm::normalize(glm::cross(Vector3::Up(), GetLook()));
}
glm::vec3 Camera::GetUp() const
{
	glm::vec3 front = GetLook();
	glm::vec3 right = GetRight();
	return glm::normalize(glm::cross(front, right));
}