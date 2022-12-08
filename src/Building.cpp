#pragma once
#include "stdafx.h"
#include "Building.h"
#include "Bullet.h"

unordered_map<BuildingType, Textures> modelMap{
	{BuildingType::Core, Textures::Core}
};

BuildingManager::Building::Building(const BuildingType& type, const glm::vec3& position, const glm::vec3 look)
{
	mCollisionType = CollisionType::Rect;
	mObject = new SharedObject(GetIdentityTextureObject(modelMap[type]));
	mObject->SetPosition(position);
	mObject->SetLook(look);

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

GLboolean BuildingManager::Building::CheckCollisionBullet(const BulletAtt& bullet, glm::vec3& hitPoint, glm::vec3& normal)
{
	switch(mCollisionType)
	{
	case CollisionType::Rect:
	{
		GLrect rect = mObject->GetRect();
		const glm::vec2 bulletCenter = { bullet.crntPos.x, bullet.crntPos.z };
		GLfloat transformedPosY = mObject->GetTransformedPos().y;
		if (::CheckCollision(rect, bulletCenter, bullet.radius) == GL_TRUE &&
			bullet.crntPos.y - bullet.radius <= transformedPosY + mObject->GetHeight() &&
			bullet.crntPos.y + bullet.radius >= transformedPosY)
		{
			const glm::vec2 prevBulletCenter = { bullet.prevPos.x, bullet.prevPos.z };

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
					hitPoint = { point.x, bullet.prevPos.y, point.y };
					if (line.v == leftTop)
					{
						normal = Vector3::Right();
					}
					else if (line.v == leftBottom)
					{
						normal = Vector3::Front();
					}
					else if (line.v == rightBottom)
					{
						normal = Vector3::Left();
					}
					else
					{
						normal = Vector3::Back();
					}

					return true;
				}
			}

			/* up-down side */
			if (bullet.prevPos.y > bullet.crntPos.y)
			{
				hitPoint = { bullet.prevPos.x, transformedPosY + mObject->GetHeight(), bullet.prevPos.z };
				normal = Vector3::Up();
			}
			else
			{
				hitPoint = { bullet.prevPos.x, transformedPosY, bullet.prevPos.z };
				normal = Vector3::Down();
			}
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