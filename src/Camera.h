#pragma once
#include "stdafx.h"
#include "Object.h"

class Camera : public Object{
private:
	GLfloat fovY = 45.0f;
	GLboolean isPersp = true;
public:
	const GLfloat speed = 10.0f;

	Camera();
	Camera(const glm::vec3& position);

	inline constexpr GLvoid SetFovY(const GLfloat& fovY)
	{
		this->fovY = fovY;
	}
	inline constexpr const GLfloat& GetFovY() const
	{
		return fovY;
	}
	inline constexpr GLvoid SetPerpective(const GLboolean& isPersp)
	{
		this->isPersp = isPersp;
	}
	inline constexpr GLvoid TogglePerpective()
	{
		isPersp = !isPersp;
	}
	inline constexpr const GLboolean& IsPersp() const
	{
		return isPersp;
	}
};