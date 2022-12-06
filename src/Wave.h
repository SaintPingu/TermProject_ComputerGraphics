#pragma once
#include "stdafx.h"

class WaveManager {
private:
	GLint crntWave = 0;

	glm::vec2 GetRandomMonsterPos(const GLint& mapWidth, const GLfloat& mapTop);
public:
	WaveManager();

	GLvoid Start();
	GLvoid Update();
};