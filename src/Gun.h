#pragma once
#include "Object.h"

enum class GunType { None=0, Red, Blue, Green, White };

class Gun {
protected:
	GunType mType = GunType::None;
	SharedObject* mObject = nullptr;
	glm::vec3 mGunPosition = { 0,0,0 };

	GLboolean mIsFire = GL_FALSE;

	GLfloat mFireDelay = 0.1f;
	GLfloat mCrntJumpDelay = 0.0f;

	GLfloat mYaw = 0.0f;
	GLfloat mPitch = 0.0f;

	GLint mAmmo = 30;
	GLint mMaxAmmo = 30;
public:
	Gun(const glm::vec3& gunPosition, const glm::vec3* pivot);

	GLvoid Update();
	virtual GLvoid Shot();
	GLvoid Draw();


	inline constexpr GLvoid StartFire() { mIsFire = GL_TRUE; }
	inline constexpr GLvoid StopFire() { mIsFire = GL_FALSE; }

	GLvoid Rotate(const GLfloat& yaw, const GLfloat& pitch);
	inline constexpr GLint GetAmmo() const { return mAmmo; };
	inline constexpr GLint GetMaxAmmo() const { return mMaxAmmo; };
	inline constexpr GunType GetType() const { return mType; };

};

class ShotGun : public Gun
{
public:
	ShotGun(const glm::vec3& gunPosition, const glm::vec3* pivot);
	GLint mBuckbullets = 5;
	GLfloat mBuckAngle = 10.0f;

	GLvoid Shot();
};

class Sniper : public Gun
{
public:
	Sniper(const glm::vec3& gunPosition, const glm::vec3* pivot);
	GLvoid Shot();
};

class Launcher : public Gun
{
public:
	Launcher(const glm::vec3& gunPosition, const glm::vec3* pivot);
	GLvoid Shot();
};