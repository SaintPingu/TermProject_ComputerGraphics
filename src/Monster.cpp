#include "stdafx.h"
#include "Monster.h"
#include "Bullet.h"
#include "Model.h"

unordered_map<MonsterType, IdentityObjects> objectMap{
	{MonsterType::Blooper, IdentityObjects::Blooper}
};

MonsterManager::Monster::Monster(const glm::vec3& position, const MonsterType& monsterType)
{
	const ModelObject* modelObject = GetIdentityModelObject(objectMap[monsterType]);
	mObject = new SharedObject(modelObject);
	mObject->SetColor(BLUE);
	mObject->SetPosition(position);

	GLfloat modelWidth = modelObject->GetWidth();
	GLfloat modelDepth = modelObject->GetDepth();

	(modelWidth > modelDepth) ? mRadius = modelWidth : mRadius = modelDepth;
	mRadius /= 2;

	extern BulletManager* bulletManager;
	bulletManager->AddCollisionObject(this);
}

GLvoid MonsterManager::Monster::Update()
{

}
GLvoid MonsterManager::Monster::Draw() const
{
	mObject->Draw();
}

GLboolean MonsterManager::Monster::CheckCollisionBullet(const glm::vec3& prevPos, const glm::vec3& bulletPos, const GLfloat& bulletRadius, const glm::vec3* hitPoint)
{
	return false;
}



MonsterManager::MonsterManager()
{
	mMonsterList.reserve(100);
}
MonsterManager::~MonsterManager()
{
	for (Monster* monster : mMonsterList)
	{
		delete monster;
	}
}
GLvoid MonsterManager::AddMonster(const glm::vec3& position, const MonsterType& monsterType)
{
	Monster* monster = new Monster(position, monsterType);
	mMonsterList.emplace_back(monster);
}
GLvoid MonsterManager::Update()
{
	for (Monster* monster : mMonsterList)
	{
		monster->Update();
	}
}
GLvoid MonsterManager::Draw() const
{
	for (Monster* monster : mMonsterList)
	{
		monster->Draw();
	}
}