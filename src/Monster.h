#pragma once
#include "stdafx.h"
#include "Object.h"

enum class MonsterType { Blooper, };

class MonsterManager {
private:
	class Monster : public IBulletCollisionable {
	private:
		SharedObject* mObject = nullptr;
		GLfloat mRadius = 0.0f;

	public:
		Monster(const glm::vec3& position, const MonsterType& monsterType);

		GLvoid Update();
		GLvoid Draw() const;

		GLboolean CheckCollisionBullet(const glm::vec3& prevPos, const glm::vec3& bulletPos, const GLfloat& bulletRadius, const glm::vec3* hitPoint = nullptr);
	};

	vector<Monster*> mMonsterList;
public:
	MonsterManager();
	~MonsterManager();
	GLvoid AddMonster(const glm::vec3& position, const MonsterType& monsterType);
	GLvoid Update();
	GLvoid Draw() const;
};