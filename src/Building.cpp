#pragma once
#include "stdafx.h"
#include "Building.h"
#include "Bullet.h"

unordered_map<BuildingType, Models> modelMap{
	{BuildingType::GuardTower, Models::GuardTower}
};

BuildingManager::Building::Building(const BuildingType& type, const glm::vec3& position, const glm::vec3 look)
{
	mCollisionType = CollisionType::Rect;
	mObject = new SharedObject(GetIdentityModelObject(modelMap[type]));
	mObject->SetPosition(position);
	mObject->SetLook(look);
	mObject->SetColor(WOOD);

	extern BulletManager* bulletManager;
	bulletManager->AddCollisionObject(this);
}
BuildingManager::Building::~Building()
{

}

GLvoid BuildingManager::Building::Draw() const
{
	mObject->Draw();
}

GLvoid BuildingManager::Building::Update()
{

}

GLboolean BuildingManager::Building::CheckCollisionBullet(const glm::vec3& prevPos, const glm::vec3& bulletPos, const GLfloat& bulletRadius, glm::vec3& hitPoint, glm::vec3& normal)
{
	switch(mCollisionType)
	{
	case CollisionType::Rect:
	{
		GLrect rect = mObject->GetRect();
		const glm::vec2 bulletCenter = { bulletPos.x, bulletPos.z };
		if (::CheckCollision(rect, bulletCenter, bulletRadius) == GL_TRUE && bulletPos.y - bulletRadius <= mObject->GetTransformedPos().y + mObject->GetHeight())
		{
			const glm::vec2 prevBulletCenter = { prevPos.x, prevPos.z };

			glm::vec2 leftTop = { rect.left, rect.top };
			glm::vec2 leftBottom = { rect.left,rect.bottom };
			glm::vec2 rightTop = { rect.right , rect.top };
			glm::vec2 rightBottom = { rect.right ,rect.bottom };

			vector<Line> lines;
			lines.emplace_back(Line(leftTop, leftBottom));
			lines.emplace_back(Line(leftBottom, rightBottom));
			lines.emplace_back(Line(rightBottom, rightTop));
			lines.emplace_back(Line(rightTop, leftTop));
			for (const Line& line : lines)
			{
				if (CheckCollision(line.v, line.u, prevBulletCenter, bulletCenter) == GL_TRUE)
				{
					glm::vec2 point = GetLineIntersection(line.v, line.u, prevBulletCenter, bulletCenter);
					hitPoint = { point.x, prevPos.y, point.y };
					normal = Vector3::Look();
					return true;
				}
			}
			return true;
			assert(0); // didn't find line intersection
		}
	}
		break;
	default:
		assert(0);
		break;
	}

	return false;
}


BuildingManager::BuildingManager()
{
	buildings.reserve(20);
}

BuildingManager::~BuildingManager()
{
	for (Building* building : buildings)
	{
		delete building;
	}
}

GLvoid BuildingManager::Update()
{
	for (Building* building : buildings)
	{
		building->Update();
	}
}
GLvoid BuildingManager::Draw() const
{
	for (Building* building : buildings)
	{
		building->Draw();
	}
}

GLvoid BuildingManager::Create(const BuildingType& type, const glm::vec3& position, const glm::vec3 look)
{
	Building* building = new Building(type, position, look);
	buildings.emplace_back(building);
}
//vector<const SharedObject*> BuildingManager::GetBuilngObjects() const
//{
//	for()
//}