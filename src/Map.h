#pragma once
#include "stdafx.h"
#include "Object.h"

class ModelObject;
class Circle;

class Map : public IBulletCollisionable {
private:
	ModelObject* mMapObject;

	glm::vec2 mLeftTop = { 0,0 };
	glm::vec2 mLeftBottom = {0,0};
	glm::vec2 mRightBottom = {0,0};
	glm::vec2 mRightTop = {0,0};
public:
	Map();

	GLvoid Draw();
	GLboolean CheckCollision(const Circle* boundingCircle);
	GLboolean CheckCollisionBullet(const glm::vec3& prevPos, const glm::vec3& bulletPos, const GLfloat& bulletRadius, const glm::vec3* hitPoint = nullptr);
};