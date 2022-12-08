#pragma once
#include "stdafx.h"
#include "Bullet.h"

enum class MonsterType { Blooper, Egg, Koromon };

class Player;
class SharedObject;

class Monster : public IBulletCollisionable {
protected:
	SharedObject* mObject = nullptr;

	GLfloat mHP = 100.0f;
	GLfloat mRadius = 0.0f;
	GLfloat mHeight = 0.0f;
	GLfloat mSpeed = 0.0f;
	GLfloat mDetectRadius = 0.0f;
	CollisionType mCollisionType = CollisionType::None;
	COLORREF mExplosionColor = WHITE;

	const glm::vec3* target = nullptr;

	GLboolean isSpawning = GL_TRUE;

public:
	Monster(const MonsterType& monsterType, const glm::vec3& position);

	virtual GLvoid Update(const glm::vec3* target);
	virtual GLvoid Look(const glm::vec3* target);
	GLvoid Draw() const;

	GLboolean CheckCollisionBullet(const BulletAtt& bullet, glm::vec3& hitPoint, glm::vec3& normal);
	glm::vec3 GetPosition() const;
	glm::vec3 GetCenter() const;

	GLvoid GetDamage(const GLfloat& damage);

	inline constexpr GLfloat GetDetectRadius() const { return mDetectRadius; }
};

class Floatable abstract {
private:
	SharedObject* mFloatingObject = nullptr;
	GLint mFloatingDir = UP;
	GLfloat mFloatingSpeed = 0.0f;
	GLfloat mFloatingRange = 0.0f;
	GLfloat mFloatingOrigin = 0.0f;
public:
	GLvoid InitFloat(SharedObject* object, const GLfloat& floatingSpeed, const GLfloat& floatingRange, const GLfloat& floatingOrigin);
	GLvoid UpdateFloat();
};

class Blooper : public Monster, Floatable {
public:
	Blooper(const MonsterType& monsterType, const glm::vec3& position);
	GLvoid Update(const glm::vec3* target) override;
};

class Egg : public Monster, Floatable {
private:
	GLfloat mRotationPerSec = 90.0f;

	
public:
	Egg(const MonsterType& monsterType, const glm::vec3& position);
	GLvoid Update(const glm::vec3* target) override;
};

class Koromon : public Monster {
private:
	const GLfloat mJumpDelay = 2.0f;
	const GLfloat mJumpTime = 1.0f;
	GLfloat mCrntJumpTime = 0.0f;
	GLfloat mCrntDelay = 0.0f;
public:
	Koromon(const MonsterType& monsterType, const glm::vec3& position);
	GLvoid Update(const glm::vec3* target) override;
};



class MonsterManager {
private:
	vector<Monster*> mMonsterList;
	Player* mPlayer = nullptr;

	const glm::vec3* FindTargetPos(const glm::vec3& monsterPos, const GLfloat& radius) const;
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