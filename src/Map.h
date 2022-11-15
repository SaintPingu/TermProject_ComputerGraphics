#include "stdafx.h"

class ModelObject;
class Circle;

class Map {
private:
	ModelObject* object;
public:
	Map();

	GLvoid Draw();
	GLboolean CheckCollision(const Circle* boundingCircle);
};