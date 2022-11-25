#include "stdafx.h"
#include "Bullet.h"
#include "Timer.h"

BulletManager::Bullet::Bullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch) : SharedObject(GetIdentityObject(IdentityObjects::LowSphere))
{
	GLfloat resultYaw = yaw + rand() % 4 - 2;
	GLfloat resultPitch = pitch + rand() % 4 - 2;

	SetScale(mRadius);
	SetColor(RED);
	SetLook(Vector3::Back());
	RotateLocal(0, resultPitch, 0);

	mPosition = position;
	mPosition.x += rand() % 2 - 1;
	mPosition.y += rand() % 2 - 1;

	mVelocity = velocity;
	mAngleY = sin(DEGREE_TO_RADIAN(resultYaw));
	mAngleZ = cos(DEGREE_TO_RADIAN(resultYaw));
}
GLvoid BulletManager::Bullet::Update()
{
	mT += timer::DeltaTime();
	MoveZ(-mVelocity * mAngleZ);
	MoveY(mVelocity * mAngleY - (0.5f * GRAVITY * mT * mT * mWeight));
}




BulletManager::BulletManager()
{
	mBullets.reserve(100);
}
BulletManager::~BulletManager()
{
	for (Bullet* bullet : mBullets)
	{
		delete bullet;
	}
}

GLvoid BulletManager::AddBullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch)
{
	Bullet* bullet = new Bullet(position, velocity, yaw, pitch);
	mBullets.emplace_back(bullet);
}

GLvoid BulletManager::Draw() const
{
	for (const Bullet* bullet : mBullets)
	{
		bullet->Draw();
	}
}
GLvoid BulletManager::Update()
{
	for(auto iter = mBullets.begin(); iter != mBullets.end();)
	{
		Bullet* bullet = (*iter);
		bullet->Update();

		GLboolean isCollision = false;
		for (IBulletCollisionable* object : mCollisionObjects)
		{
			if (object->CheckCollisionBullet(bullet->GetTransformedPos(), bullet->GetRadius()) == true)
			{
				delete bullet;
				iter = mBullets.erase(iter);
				isCollision = true;
				break;
			}
		}

		if (isCollision == false)
		{
			++iter;
		}
	}
}

