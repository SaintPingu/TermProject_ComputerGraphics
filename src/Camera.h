#include "stdafx.h"

class Camera {
private:
	glm::vec3 look = { 0.0f, 0.0f, -1.0f };
	glm::vec3 position = { 0.0f,0.0f,0.0f };

	GLfloat fovY = 45.0f;
	GLboolean isPersp = true;
	const glm::vec3* pivot = nullptr;
public:
	const GLfloat speed = 10.0f;

	Camera();
	Camera(const glm::vec3& position);

	GLvoid SetPosition(const glm::vec3& position);

	GLvoid Move(const glm::vec3& vector);
	GLvoid MoveX(const GLfloat& amount);
	GLvoid MoveY(const GLfloat& amount);
	GLvoid MoveZ(const GLfloat& amount);
	GLvoid MoveGlobal(const glm::vec3& offset);

	GLvoid Look(const glm::vec3& point);
	GLvoid SetLook(const glm::vec3& look);
	GLvoid RotateAxis(const GLfloat& theta, const glm::vec3& axis);
	GLvoid RotateLocal(const GLfloat& x, const GLfloat& y, const GLfloat& z);
	GLvoid RotateYAxis(const GLfloat& theta);
	GLvoid RotatePosition(const glm::vec3& axis, const GLfloat& degree);

	GLvoid SetPivot(const glm::vec3* pivot);
	glm::vec3 GetPosition() const;
	glm::vec3 GetLook() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;

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