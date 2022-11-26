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

		const glm::vec3* target = nullptr;

	public:
		Monster(const glm::vec3& position, const MonsterType& monsterType);

		GLvoid Update(const glm::vec3* target);
		GLvoid Draw() const;

		GLboolean CheckCollisionBullet(const glm::vec3& prevPos, const glm::vec3& bulletPos, const GLfloat& bulletRadius, const glm::vec3* hitPoint = nullptr);
		glm::vec3 GetPosition() const;
	};

	vector<Monster*> mMonsterList;
	const Player* mPlayer = nullptr;
public:
	MonsterManager();
	~MonsterManager();
	GLvoid AddMonster(const glm::vec3& position, const MonsterType& monsterType);
	GLvoid Update();
	GLvoid Draw() const;
	GLvoid SetPlayer(const Player* player);
};