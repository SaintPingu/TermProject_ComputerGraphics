#include "stdafx.h"
#include "Bullet.h"
#include "Timer.h"

BulletManager::Bullet::Bullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch) : SharedObject(GetIdentitySphere())
{
	SetScale(0.1f);
	SetColor(RED);
	this->position = position;
	mVelocity = velocity;
	SetLook(Vector3::Back());
	RotateLocal(0, pitch, 0);
	RotateLocal(-yaw, 0, 0);
}
GLvoid BulletManager::Bullet::Update()
{
	MoveZ(-mVelocity);
}




BulletManager::BulletManager()
{
	bullets.reserve(100);
}
BulletManager::~BulletManager()
{
	for (Bullet* bullet : bullets)
	{
		delete bullet;
	}
}

GLvoid BulletManager::AddBullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch)
{
	Bullet* bullet = new Bullet(position, velocity, yaw, pitch);
	bullets.emplace_back(bullet);
	AddObject(Shader::Light, bullet);
}

GLvoid BulletManager::Update()
{
	for (Bullet* bullet : bullets)
	{
		bullet->Update();
	}
}

