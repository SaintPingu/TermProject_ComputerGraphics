#include "stdafx.h"
#include "Wave.h"
#include "Monster.h"
#include "Map.h"
#include <random>


extern MonsterManager* monsterManager;
extern Map* crntMap;

WaveManager::WaveManager()
{
	crntWave = 1;
}

glm::vec2 WaveManager::GetRandomMonsterPos(const GLint& mapWidth, const GLfloat& mapTop)
{
	GLfloat posX = rand() % mapWidth - (mapWidth / 2.0f);
	GLfloat posZ = rand() % 50 + mapTop;

	return glm::vec2(posX, posZ);
}

GLvoid WaveManager::Start()
{


	GLint mapWidth = static_cast<GLint>(crntMap->GetMaxWidth());
	GLfloat mapTop = crntMap->GetTop();


	const GLuint numOfBlooper = crntWave * 3;
	const GLuint numOfEgg = crntWave * 3;

	for (GLuint i = 0; i < numOfBlooper; ++i)
	{
		glm::vec2 pos = GetRandomMonsterPos(mapWidth, mapTop);
		monsterManager->Create(MonsterType::Blooper, glm::vec3(pos.x, 20, pos.y));
	}

	for (GLuint i = 0; i < numOfEgg; ++i)
	{
		glm::vec2 pos = GetRandomMonsterPos(mapWidth, mapTop);
		monsterManager->Create(MonsterType::Egg, glm::vec3(pos.x, 40, pos.y));
	}
}

GLvoid WaveManager::Update()
{
	if (monsterManager->CheckEnemyEmpty())
	{
		crntWave++;

		WaveManager::Start();
		cout << crntWave << endl;
	}
}