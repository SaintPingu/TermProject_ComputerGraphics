#pragma once
#include "Object.h"

enum class GunType { None=0, Red, Blue, Green, White };

class Gun {
private:
	GunType mType = GunType::None;
	SharedObject* mObject = nullptr;
	glm::vec3 mGunPosition = { 0,0,0 };

	GLboolean mIsFire = GL_FALSE;

	GLfloat mFireDelay = 0.1f;
	GLfloat mCrntDelay = 0.0f;

	GLfloat mYaw = 0.0f;
	GLfloat mPitch = 0.0f;

	GLint mAmmo = 60;
public:
	Gun(const glm::vec3& gunPosition, const glm::vec3* pivot);

	GLvoid Update();

	inline constexpr GLvoid StartFire() { mIsFire = GL_TRUE; }
	inline constexpr GLvoid StopFire() { mIsFire = GL_FALSE; }

	GLvoid Rotate(const GLfloat& yaw, const GLfloat& pitch);
	inline constexpr GLint GetAmmo() const { return mAmmo; };
};