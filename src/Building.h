#pragma once
#include "stdafx.h"
#include "Object.h"

enum class BuildingType { GuardTower, };

class BuildingManager {
private:
	class Building : IBulletCollisionable {
	private:
		CollisionType mCollisionType = CollisionType::None;
		SharedObject* mObject = nullptr;
	public:
		Building(const BuildingType& type, const glm::vec3& position, const glm::vec3 look);
		~Building();

		GLvoid Update();
		GLvoid Draw() const;

		GLboolean CheckCollisionBullet(const glm::vec3& prevPos, const glm::vec3& bulletPos, const GLfloat& bulletRadius, const glm::vec3* hitPoint = nullptr);
	};

	vector<Building*> buildings;
public:
	BuildingManager();
	~BuildingManager();

	GLvoid Update();
	GLvoid Draw() const;

	GLvoid Create(const BuildingType& type, const glm::vec3& position, const glm::vec3 look = Vector3::Look());
};