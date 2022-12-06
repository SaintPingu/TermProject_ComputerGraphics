#pragma once
#include "stdafx.h"
#include "Bullet.h"

enum class MonsterType { Blooper, Egg };

class Player;
class SharedObject;

class Monster : public IBulletCollisionable {
protected:
	SharedObject* mObject = nullptr;

	GLfloat mHP = 100.0f;
	GLfloat mRadius = 0.0f;
	GLfloat mHeight = 0.0f;
	GLfloat mSpeed = 0.0f;
	CollisionType mCollisionType = CollisionType::None;
	COLORREF mExplosionColor = WHITE;

	const glm::vec3* target = nullptr;

public:
	Monster(const MonsterType& monsterType, const glm::vec3& position);

	virtual GLvoid Update(const glm::vec3* target);
	GLvoid Draw() const;

	GLboolean CheckCollisionBullet(const BulletAtt& bullet, glm::vec3& hitPoint, glm::vec3& normal);
	glm::vec3 GetPosition() const;
	glm::vec3 GetCenter() const;

	GLvoid GetDamage(const GLfloat& damage);
};

class Blooper : public Monster {
public:
	Blooper(const MonsterType& monsterType, const glm::vec3& position);
};

class Egg : public Monster {
private:
	GLfloat mRotationPerSec = 90.0f;

	GLint mFloatingDir = UP;
	GLfloat mFloatingSpeed = 10.0f;
	GLfloat mFloatingRange = 10.0f;
	GLfloat mFloatingOrigin = 0.0f;
public:
	Egg(const MonsterType& monsterType, const glm::vec3& position);

	virtual GLvoid Update(const glm::vec3* target);
};




class MonsterManager {
private:
	vector<Monster*> mMonsterList;
	Player* mPlayer = nullptr;

	const glm::vec3* FindTargetPos(const glm::vec3& monsterPos) const;
public:
	MonsterManager();
	~MonsterManager();
	GLvoid Create(const MonsterType& monsterType, const glm::vec3& position);
	GLvoid Update();
	GLvoid Draw() const;
	GLvoid SetPlayer(Player* player);
	GLboolean GetShortestMonsterPos(const glm::vec3& srcPos, const GLfloat& radius, glm::vec3& targetPos) const;
	GLvoid CheckPlayerCollision(const Monster* monster);
};