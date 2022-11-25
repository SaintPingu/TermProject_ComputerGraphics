#include "stdafx.h"
#include "Bullet.h"
#include "Timer.h"

BulletManager::Bullet::Bullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch) : SharedObject(GetIdentityObject(IdentityObjects::LowSphere))
{
	SetScale(0.1f);
	SetColor(RED);
	SetLook(Vector3::Back());
	RotateLocal(0, pitch, 0);

	mPosition = position;
	mVelocity = velocity;
	mAngleY = sin(DEGREE_TO_RADIAN(yaw));
	mAngleZ = cos(DEGREE_TO_RADIAN(yaw));
}
GLvoid BulletManager::Bullet::Update()
{
	mT += timer::DeltaTime() * 3.0f;
	MoveZ(-mVelocity * mAngleZ);
	MoveY(mVelocity * mAngleY - (0.5f * GRAVITY * mT * mT));
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
	AddObject(Shader::Light, bullet);
}

GLvoid BulletManager::Update()
{
	for (Bullet* bullet : mBullets)
	{
		bullet->Update();
	}
}

