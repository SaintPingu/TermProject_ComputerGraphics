#pragma once
#include "stdafx.h"
#include "Object.h"

class BulletManager {
private:
	class Bullet : public SharedObject {
		/* �߷��� ������ �޴� ���� (n��) */
		const GLfloat mGravityEffected = 5.0f;
		GLfloat mT = 0.0f;
		GLfloat mAngleY = 0.0f;
		GLfloat mAngleZ = 0.0f;
		GLfloat mVelocity = 0.0f;
	public:
		Bullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch);
		GLvoid Update();
	};

	vector<Bullet*> mBullets;
public:
	BulletManager();
	~BulletManager();
	GLvoid AddBullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch);
	GLvoid Update();
};