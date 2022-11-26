#include "stdafx.h"
#include "Bullet.h"
#include "Timer.h"

BulletManager::Bullet::Bullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch) : SharedObject(GetIdentityModelObject(IdentityObjects::LowSphere))
{
	GLfloat resultYaw = yaw + rand() % (mSpreadAmount*2) - mSpreadAmount;
	GLfloat resultPitch = pitch + rand() % (mSpreadAmount*2) - mSpreadAmount;

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
	mPrevPos = GetTransformedPos();

	/* https://www.101computing.net/projectile-motion-formula/ */
	mT += timer::DeltaTime();
	MoveZ(-mVelocity * mAngleZ);
	MoveY(mVelocity * mAngleY - (0.5f * GRAVITY * mT * mT * mWeight));
}




BulletManager::BulletManager()
{
	mBulletList.reserve(100);
}
BulletManager::~BulletManager()
{
	for (Bullet* bullet : mBulletList)
	{
		delete bullet;
	}
}

GLvoid BulletManager::AddBullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch)
{
	Bullet* bullet = new Bullet(position, velocity, yaw, pitch);
	mBulletList.emplace_back(bullet);
}

GLvoid BulletManager::Draw() const
{
	for (const Bullet* bullet : mBulletList)
	{
		bullet->Draw();
	}
}
GLvoid BulletManager::Update()
{
	for(auto iter = mBulletList.begin(); iter != mBulletList.end();)
	{
		Bullet* bullet = (*iter);
		bullet->Update();

		GLboolean isCollision = false;
		for (IBulletCollisionable* object : mCollisionObjectList)
		{
			if (object->CheckCollisionBullet(bullet->GetPrevPos(), bullet->GetTransformedPos(), bullet->GetRadius()) == GL_TRUE)
			{
				delete bullet;
				iter = mBulletList.erase(iter);
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

