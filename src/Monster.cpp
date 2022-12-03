#include "stdafx.h"
#include "Monster.h"
#include "Bullet.h"
#include "Player.h"

unordered_map<MonsterType, Models> modelMap{
	{MonsterType::Blooper, Models::Blooper}
};

MonsterManager::Monster::Monster(const glm::vec3& position, const MonsterType& monsterType)
{
	mCollisionType = CollisionType::Circle;

	const ModelObject* modelObject = GetIdentityModelObject(modelMap[monsterType]);
	mObject = new SharedObject(modelObject);
	mObject->SetColor(BLUE);
	mObject->SetPosition(position);
	mObject->SetLook(Vector3::Up());

	GLfloat modelWidth = modelObject->GetWidth();
	GLfloat modelDepth = modelObject->GetDepth();
	mHeight = modelObject->GetHeight();

	(modelWidth > modelDepth) ? mRadius = modelWidth : mRadius = modelDepth;
	mRadius /= 2;

	mSpeed = 10.0f;

	extern BulletManager* bulletManager;
	bulletManager->AddCollisionObject(this);
}

GLvoid MonsterManager::Monster::Update(const glm::vec3* target)
{
	glm::vec3 monsterPos = mObject->GetPosition();
	glm::vec3 v = { target->x, monsterPos.y, target->z };
	glm::vec3 u = monsterPos;

	glm::vec3 look = glm::normalize(v - u);
	mObject->SetLook(look);

	mObject->MoveZ(mSpeed);
}
GLvoid MonsterManager::Monster::Draw() const
{
	mObject->Draw();
}

GLboolean MonsterManager::Monster::CheckCollisionBullet(const glm::vec3& prevPos, const glm::vec3& bulletPos, const GLfloat& bulletRadius, glm::vec3& hitPoint, glm::vec3& normal)
{
	normal.x = -9;
	switch (mCollisionType)
	{
	case CollisionType::Circle:
	{
		glm::vec3 monsterPos = mObject->GetTransformedPos();
		return ::CheckCollision(monsterPos, bulletPos, mRadius, bulletRadius, mHeight);
	}
	default:
		assert(0);
		break;
	}

	return false;
}
glm::vec3 MonsterManager::Monster::GetPosition() const
{
	return mObject->GetPosition();
}





const glm::vec3* MonsterManager::FindTargetPos(const glm::vec3& monsterPos) const
{
	const glm::vec3* target = nullptr;

	glm::vec2 playerCenter = { mPlayer->GetPosition().x , mPlayer->GetPosition().z };
	glm::vec2 monsterCenter = { monsterPos.x, monsterPos.z };
	GLfloat minDistance = glm::length(playerCenter - monsterCenter);



	target = mPlayer->GetRefPos();

	return target;
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
GLvoid MonsterManager::Create(const MonsterType& monsterType, const glm::vec3& position)
{
	Monster* monster = new Monster(position, monsterType);
	mMonsterList.emplace_back(monster);
}
GLvoid MonsterManager::Update()
{
	for (Monster* monster : mMonsterList)
	{
		const glm::vec3* target = FindTargetPos(monster->GetPosition());

		monster->Update(target);
	}
}
GLvoid MonsterManager::Draw() const
{
	for (Monster* monster : mMonsterList)
	{
		monster->Draw();
	}
}

GLvoid MonsterManager::SetPlayer(const Player* player)
{
	mPlayer = player;
}