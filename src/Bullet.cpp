#include "stdafx.h"
#include "Bullet.h"
#include "Timer.h"


GLvoid IBulletCollisionable::Destroy()
{
	extern BulletManager* bulletManager;
	isDestroyed = GL_TRUE;
	bulletManager->DelCollisionObject(this);
};


BulletManager::Bullet::Bullet(const BulletType& type, const glm::vec3& origin, const glm::vec3& position, const GLfloat& yaw, const GLfloat& pitch) : SharedObject(GetIdentityModelObject(Models::LowSphere))
{
	switch (type)
	{
	case BulletType::Normal:
		mWeight = 30.0f;
		mRadius = 0.1f;
		mVelocity = 300.0f;
		mDamage = 10.0f;
		break;
	default:
		assert(0);
		break;
	}

	GLfloat resultYaw = yaw + rand() % (mSpreadAmount*2) - mSpreadAmount;
	GLfloat resultPitch = pitch + rand() % (mSpreadAmount*2) - mSpreadAmount;

	SetScale(mRadius);
	SetColor(RED);
	RotateLocal(0, resultPitch, 0);

	mPrevPos = origin;
	mPosition = position;
	mPosition.x += rand() % 2 - 1;
	mPosition.y += rand() % 2 - 1;

	mAngleY = sin(DEGREE_TO_RADIAN(resultYaw));
	mAngleZ = cos(DEGREE_TO_RADIAN(resultYaw));

}
GLvoid BulletManager::Bullet::Update()
{
	mPrevPos = GetTransformedPos();

	/* https://www.101computing.net/projectile-motion-formula/ */
	mT += timer::DeltaTime();
	MoveZ(mVelocity * mAngleZ);
	MoveY(mVelocity * mAngleY - (0.5f * GRAVITY * mT * mT * mWeight));
}
BulletAtt BulletManager::Bullet::GetAttribute() const
{
	BulletAtt result;
	result.prevPos = mPrevPos;
	result.crntPos = GetTransformedPos();
	result.radius = mRadius;
	result.damage = mDamage;

	return result;
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

GLvoid BulletManager::Create(const BulletType& type, const glm::vec3& origin, const glm::vec3& position, const GLfloat& yaw, const GLfloat& pitch)
{
	Bullet* bullet = new Bullet(type, origin, position, yaw, pitch);
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
	constexpr GLfloat NO_NORMAL = 9;

	for(auto iter = mBulletList.begin(); iter != mBulletList.end();)
	{
		Bullet* bullet = (*iter);

		for (IBulletCollisionable* object : mCollisionObjectList)
		{
			glm::vec3 hitPoint;
			glm::vec3 normal = { NO_NORMAL, NO_NORMAL, NO_NORMAL };

			if (object->CheckCollisionBullet(bullet->GetAttribute(), hitPoint, normal) == GL_TRUE)
			{
				delete bullet;
				bullet = nullptr;
				iter = mBulletList.erase(iter);

				/* create paint */
				if (normal.x != NO_NORMAL)
				{
					PaintPlane* plane = new PaintPlane(hitPoint, normal);
					mPaints.emplace_back(plane);
				}

				break;
			}
		}

		/* didn't collision */
		if (bullet != nullptr)
		{
			bullet->Update();
			++iter;
		}
	}

	for (auto iter = mPaints.begin(); iter != mPaints.end();)
	{
		PaintPlane* paint = *iter;
		if (paint->Update() == false)
		{
			delete paint;
			iter = mPaints.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

