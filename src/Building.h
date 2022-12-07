#pragma once
#include "stdafx.h"
#include "Bullet.h"

class SharedObject;

enum class BuildingType { Core };

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

		GLboolean CheckCollisionBullet(const BulletAtt& bullet, glm::vec3& hitPoint, glm::vec3& normal);
		const SharedObject* GetBuildingObject() const { return mObject; }
	};

	vector<Building*> buildings;
public:
	BuildingManager();
	~BuildingManager();

	GLvoid Update();
	GLvoid Draw() const;

	GLvoid Create(const BuildingType& type, const glm::vec3& position, const glm::vec3 look = Vector3::Front());
};