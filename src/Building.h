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
		GLrect mRect{};
		GLfloat mRadius = 0.0f;
		glm::vec2 mCenter = glm::vec2(0, 0);

	public:
		Building(const BuildingType& type, const glm::vec3& position, const glm::vec3 look);
		~Building();

		GLvoid Update();
		GLvoid Draw() const;

		GLboolean CheckCollision(const Circle* boundingCircle) const;
		GLboolean CheckCollisionBullet(const BulletAtt& bullet, glm::vec3& hitPoint, glm::vec3& normal);
		const SharedObject* GetBuildingObject() const { return mObject; }
	};

	vector<Building*> buildings;
	const glm::vec3* mCorePos = nullptr;
public:
	BuildingManager();
	~BuildingManager();

	GLvoid Update();
	GLvoid Draw() const;

	GLvoid Create(const BuildingType& type, const glm::vec3& position, const glm::vec3 look = Vector3::Front());
	GLboolean CheckCollision(const Circle* boundingCircle) const;
	inline constexpr const glm::vec3* GetCorePos() const { return mCorePos; }
};