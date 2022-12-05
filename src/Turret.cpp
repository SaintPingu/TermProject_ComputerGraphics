#pragma once
#include "stdafx.h"
#include "Turret.h"
#include "Object.h"
#include "Bullet.h"
#include "Monster.h"
#include "Timer.h"

extern MonsterManager* monsterManager;
extern BulletManager* bulletManager;

TurretManager::Turret::Turret(const glm::vec3& position)
{
	mObject_Body = new SharedObject(GetIdentityTextureObject(TextureModels::Turret_Body));
	mObject_Head = new SharedObject(GetIdentityTextureObject(TextureModels::Turret_Head));
	mObject_Body->SetPosition(position);
	mObject_Head->SetPosition(position);
	mObject_Head->MoveY(10, false);
}
TurretManager::Turret::~Turret()
{

}

GLvoid TurretManager::Turret::Draw() const
{
	mObject_Body->Draw();
	mObject_Head->Draw();
}

GLvoid TurretManager::Turret::Update()
{
	mCrntDelay += timer::DeltaTime();

	glm::vec3 targetPos;
	if (monsterManager->GetShortestMonsterPos(mObject_Head->GetPosition(), mRadius, targetPos) == GL_TRUE)
	{
		mObject_Head->Look(targetPos);

		if (mCrntDelay >= mFireDelay)
		{
			mCrntDelay = 0;
			glm::vec3 originPos = glm::vec3(0, 0, 0);
			glm::vec3 bulletPos = glm::vec3(0, 0, 15);
			MultiplyVector(mObject_Head->GetTransform(), bulletPos);
			MultiplyVector(mObject_Head->GetTransform(), originPos);

			GLfloat yaw = 0.0f;
			GLfloat pitch = 0.0f;
			GetYawPitch(mObject_Head->GetLook(), yaw, pitch);

			bulletManager->Create(BulletType::Normal, originPos, bulletPos, yaw, pitch);
		}
	}
	else
	{
		mObject_Head->SetLook(Vector3::Front());
	}
}



TurretManager::TurretManager()
{
	turrets.reserve(20);
}

TurretManager::~TurretManager()
{
	for (Turret* turret : turrets)
	{
		delete turret;
	}
}

GLvoid TurretManager::Update()
{
	for (Turret* turret : turrets)
	{
		turret->Update();
	}
}
GLvoid TurretManager::Draw() const
{
	for (const Turret* turret : turrets)
	{
		turret->Draw();
	}
}

GLvoid TurretManager::Create(const glm::vec3& position)
{
	Turret* turret = new Turret(position);
	turrets.emplace_back(turret);
}