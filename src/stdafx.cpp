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





GLboolean CheckCollision(const glm::vec2& start, const glm::vec2& end, const glm::vec2& center, const GLfloat& radius)
{
	glm::vec2 dir = end - start;	// start to end
	glm::vec2 f = start - center;	// center to start

	GLfloat a = glm::dot(dir, dir);
	GLfloat b = 2 * glm::dot(f, dir);
	GLfloat c = glm::dot(f, f) - (radius * radius);

	GLfloat discriminant = (b * b) - 4 * a * c;

	if (discriminant < 0)
	{
		return false; // no intersection
	}
	else
	{
		// ray didn't totally miss sphere, so there is a solution to the equation.
		discriminant = sqrtf(discriminant);

		// either solution may be on or off the ray so need to test both
		// t1 is always the smaller value, because BOTH discriminant and a are non-negative.
		GLfloat t1 = (-b - discriminant) / (2 * a);
		GLfloat t2 = (-b + discriminant) / (2 * a);

		if (t1 >= 0 && t1 <= 1)
		{
			return true;
		}
		if (t2 >= 0 && t2 <= 1)
		{
			return true;
		}
	}

	return false;
}

/* https://gamedev.stackexchange.com/questions/26004/how-to-detect-2d-line-on-line-collision */
GLboolean CheckCollision(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& u1, const glm::vec2& u2)
{
	GLfloat denominator = ((v2.x - v1.x) * (u2.y - u1.y)) - ((v2.y - v1.y) * (u2.x - u1.x));
	GLfloat numerator1 = ((v1.y - u1.y) * (u2.x - u1.x)) - ((v1.x - u1.x) * (u2.y - u1.y));
	GLfloat numerator2 = ((v1.y - u1.y) * (v2.x - v1.x)) - ((v1.x - u1.x) * (v2.y - v1.y));

	if (denominator == 0)
	{
		return (numerator1 == 0 && numerator2 == 0);
	}

	GLfloat r = numerator1 / denominator;
	GLfloat s = numerator2 / denominator;

	return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

void SetConsoleCursor(short x, short y)
{
	COORD cursor = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}