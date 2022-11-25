#include "stdafx.h"
#include "Map.h"
#include "Model.h"
#include "Object.h"

extern const Model* mapModel;

Map::Map()
{
	mObject = new ModelObject(mapModel);
	mObject->SetColor(DARK_GREEN);
	mObject->BindBuffers();
}

GLvoid Map::Draw()
{
	mObject->Draw();
}

GLboolean Map::CheckCollision(const Circle* boundingCircle)
{
	set<glm::vec2, CompareSet> boundingMap = mObject->GetBoundings_XZ();
	auto iter = boundingMap.begin();
	glm::vec2 leftTop = *(iter++);
	glm::vec2 leftBottom = *(iter++);
	glm::vec2 rightBottom = *(iter++);
	glm::vec2 rightTop = *(iter++);

	glm::vec2 center = boundingCircle->GetCenter();
	GLfloat radius = boundingCircle->GetRadius();

	if (center.y - radius <= leftTop.y)
	{
		return true;
	}
	if (center.y + radius >= leftBottom.y)
	{
		return true;
	}
	if (center.x - radius < leftBottom.x)
	{
		if (::CheckCollision(leftTop, leftBottom, center, radius) == GL_TRUE)
		{
			return true;
		}
	}
	else if (center.x + radius > rightBottom.x)
	{
		if (::CheckCollision(rightTop, rightBottom, center, radius) == GL_TRUE)
		{
			return true;
		}
	}

	return false;
}