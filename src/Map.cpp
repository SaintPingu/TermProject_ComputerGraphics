#include "stdafx.h"
#include "Map.h"
#include "Bullet.h"

Map::Map()
{
	mMapObject = new ModelObject(GetModel(Models::Map), Shader::Light);
	mMapObject->SetColor(DARK_GREEN);
	mMapObject->BindBuffers();

	set<glm::vec2, CompareSet> boundingMap = mMapObject->GetBoundings_XZ();
	auto iter = boundingMap.begin();
	mLeftTop = *(iter++);
	mLeftBottom = *(iter++);
	mRightBottom = *(iter++);
	mRightTop = *(iter++);

	extern BulletManager* bulletManager;
	bulletManager->AddCollisionObject(this);
}

GLvoid Map::Draw()
{
	mMapObject->Draw();
}

GLboolean Map::CheckCollision(const Circle* boundingCircle)
{
	glm::vec2 center = boundingCircle->GetCenter();
	GLfloat radius = boundingCircle->GetRadius();

	if (center.y - radius <= mLeftTop.y)
	{
		return true;
	}
	if (center.y + radius >= mLeftBottom.y)
	{
		return true;
	}
	if (center.x - radius < mLeftBottom.x)
	{
		if (::CheckCollision(mLeftTop, mLeftBottom, center, radius) == GL_TRUE)
		{
			return true;
		}
	}
	else if (center.x + radius > mRightBottom.x)
	{
		if (::CheckCollision(mRightTop, mRightBottom, center, radius) == GL_TRUE)
		{
			return true;
		}
	}

	return false;
}

GLboolean Map::CheckCollisionBullet(const glm::vec3& prevPos, const glm::vec3& bulletPos, const GLfloat& bulletRadius, const glm::vec3* hitPoint)
{
	if (bulletPos.y < 0)
	{
		return true;
	}

	// check collision with previous to current bullet position line

	const glm::vec2 center = Get2D(bulletPos);
	const glm::vec2 prevCenter = Get2D(prevPos);
	if (center.y - bulletRadius <= mLeftTop.y)
	{
		return true;
	}
	if (center.y + bulletRadius >= mLeftBottom.y)
	{
		return true;
	}
	if (center.x - bulletRadius < mLeftBottom.x)
	{
		if (::CheckCollision(mLeftTop, mLeftBottom, prevCenter, center) == GL_TRUE)
		{
			return true;
		}
	}
	else if (center.x + bulletRadius > mRightBottom.x)
	{
		if (::CheckCollision(mRightTop, mRightBottom, prevCenter, center) == GL_TRUE)
		{
			return true;
		}
	}

	
	

	return false;
}