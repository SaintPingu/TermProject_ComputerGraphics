#include "stdafx.h"
#include "Monster.h"
#include "Object.h"
#include "Player.h"

extern BulletManager* bulletManager;

unordered_map<MonsterType, TextureModels> modelMap{
	{MonsterType::Blooper, TextureModels::Blooper}
};

MonsterManager::Monster::Monster(const glm::vec3& position, const MonsterType& monsterType)
{
	mCollisionType = CollisionType::Circle;

	const ModelObject* modelObject = GetIdentityTextureObject(modelMap[monsterType]);
	mObject = new SharedObject(modelObject);
	mObject->SetPosition(position);

	GLfloat modelWidth = modelObject->GetWidth();
	GLfloat modelDepth = modelObject->GetDepth();
	mHeight = modelObject->GetHeight();

	(modelWidth > modelDepth) ? mRadius = modelWidth : mRadius = modelDepth;
	mRadius /= 2;

	mSpeed = 10.0f;

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

GLboolean MonsterManager::Monster::CheckCollisionBullet(const BulletAtt& bullet, glm::vec3& hitPoint, glm::vec3& normal)
{
	switch (mCollisionType)
	{
	case CollisionType::Circle:
	{
		glm::vec3 monsterPos = mObject->GetTransformedPos();
		if (::CheckCollision(monsterPos, bullet.crntPos, mRadius, bullet.radius, mHeight) == GL_TRUE)
		{
			GetDamage(bullet.damage);
			return true;
		}
	}
	break;
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
glm::vec3 MonsterManager::Monster::GetCenter() const
{
	glm::vec3 pos = mObject->GetPosition();
	return glm::vec3(pos.x, pos.y + mObject->GetHeight() / 2.0f, pos.z);
}
GLvoid MonsterManager::Monster::GetDamage(const GLfloat& damage)
{
	mHP -= damage;
	if (mHP <= 0)
	{
		Destroy();
	}
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
	for (auto it = mMonsterList.begin(); it != mMonsterList.end();)
	{
		Monster* monster = *it;
		if (monster->IsDestroyed() == GL_TRUE)
		{
			it = mMonsterList.erase(it);
		}
		else
		{
			const glm::vec3* target = FindTargetPos(monster->GetPosition());
			monster->Update(target);


			++it;
		}
	}
}
GLvoid MonsterManager::Draw() const
{
	for (const Monster* monster : mMonsterList)
	{
		monster->Draw();
	}
}

GLvoid MonsterManager::SetPlayer(Player* player)
{
	mPlayer = player;
}

GLboolean MonsterManager::GetShortestMonsterPos(const glm::vec3& srcPos, const GLfloat& radius, glm::vec3& targetPos) const
{
	GLfloat min = radius;

	for (const Monster* monster : mMonsterList)
	{
		glm::vec3 monsterPos = monster->GetPosition();
		GLfloat length = glm::length(monsterPos - srcPos);
		if (length < min)
		{
			min = length;
			targetPos = monster->GetCenter();
		}
	}

	if (min >= radius)
	{
		return false;
	}

	return true;
}

GLvoid MonsterManager::CheckPlayerCollision(const Monster* monster)
{
}