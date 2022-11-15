#include "stdafx.h"

extern GLint screenPosX;
extern GLint screenPosY;
extern GLsizei screenWidth;
extern GLsizei screenHeight;





extern GLpoint mouseCenter;
GLvoid Reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	screenWidth = w;
	screenHeight = h;
	
	mouseCenter = { screenWidth / 2 + screenPosX, screenHeight / 2 + screenPosY };
}
GLvoid RePosition(GLint x, GLint y)
{
	screenPosX = x;
	screenPosY = y;

	mouseCenter = { screenWidth / 2 + screenPosX, screenHeight / 2 + screenPosY };
}
GLfloat GetRandColor()
{
	GLint color = GetRandom<GLint>(10000);
	return GLfloat(color * 0.0001);
}
COLORREF GetRandRGB()
{
	GLint r = GetRandom<GLint>(256);
	GLint g = GetRandom<GLint>(256);
	GLint b = GetRandom<GLint>(256);

	return RGB(r, g, b);
}

GLvoid ChangePosToGL(GLfloat& x, GLfloat& y)
{
	x /= screenWidth;
	y /= screenHeight;

	x *= 2;
	y *= 2;

	x -= 1.0f;
	y -= 1.0f;
	y *= -1;
}
Vector2 GetPosToGL(const GLint& x, const GLint& y)
{
	GLfloat toX = (GLfloat)x;
	GLfloat toY = (GLfloat)y;
	ChangePosToGL(toX, toY);
	return { toX, toY };
}
GLvoid CheckOutOfIndex(const size_t& index, const size_t& size)
{
	if (index >= size)
	{
		assert(0);
	}
}
GLboolean IsOutOfIndex(const size_t& index, const size_t& size)
{
	if (index >= size)
	{
		return false;
	}

	return true;
}

GLvoid CheckOutOfIndex(const GLint& index, const GLint& min, const GLint& max)
{
	if (index < min || index >= max)
	{
		assert(0);
	}
}

GLvoid ToggleDepthTest()
{
	if (glIsEnabled(GL_DEPTH_TEST))
	{
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
	}
}
GLvoid SetDepthTest(const GLboolean& isDepthTest)
{
	if (isDepthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

GLvoid MultiplyVector(const glm::mat4& transform, glm::vec3& vector)
{
	glm::vec4 v({ vector , 1.0f });
	v = transform * v;

	vector = { v.x, v.y, v.z };
}

GLvoid Rotate(glm::vec3& vector, const GLfloat& theta, const glm::vec3& axis)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, theta, axis);
	MultiplyVector(transform, vector);
}

GLvoid RotatePosition(glm::vec3& position, const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(degree), glm::normalize(axis));
	position -= pivot;
	MultiplyVector(transform, position);
	position += pivot;
}








void SetConsoleCursor(int x, int y)
{
	COORD cursor = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}