#pragma once
#include "stdafx.h"
#include "Object.h"

enum class MonsterType { Blooper, };

class Player;

class MonsterManager {
private:
	class Monster : public IBulletCollisionable {
	private:
		SharedObject* mObject = nullptr;
		GLfloat mRadius = 0.0f;
		GLfloat mHeight = 0.0f;
		GLfloat mSpeed = 0.0f;
		CollisionType mCollisionType = CollisionType::None;

		const glm::vec3* target = nullptr;

	public:
		Monster(const glm::vec3& position, const MonsterType& monsterType);

		GLvoid Update(const glm::vec3* target);
		GLvoid Draw() const;

		GLboolean CheckCollisionBullet(const glm::vec3& prevPos, const glm::vec3& bulletPos, const GLfloat& bulletRadius, glm::vec3& hitPoint, glm::vec3& normal);
		glm::vec3 GetPosition() const;
	};

	vector<Monster*> mMonsterList;
	const Player* mPlayer = nullptr;

	const glm::vec3* FindTargetPos(const glm::vec3& monsterPos) const;
public:
	MonsterManager();
	~MonsterManager();
	GLvoid Create(const MonsterType& monsterType, const glm::vec3& position);
	GLvoid Update();
	GLvoid Draw() const;
	GLvoid SetPlayer(const Player* player);
};