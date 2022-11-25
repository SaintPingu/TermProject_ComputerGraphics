#pragma once
#include "stdafx.h"

class ModelObject;
class Circle;

class Map {
private:
	ModelObject* mObject;
public:
	Map();

	GLvoid Draw();
	GLboolean CheckCollision(const Circle* boundingCircle);
};