#pragma once
#include "Object.h"

enum class GunType { None=0, Red, Blue, Green, White };

class Gun {
private:
	GunType mType = GunType::None;
	SharedObject* mObject = nullptr;
	glm::vec3 mGunPosition = { 0,0,0 };

	GLboolean mIsFire = false;

	GLfloat mFireDelay = 0.1f;
	GLfloat mCrntDelay = 0.0f;

	GLfloat mYaw = 0.0f;
	GLfloat mPitch = 0.0f;
public:
	Gun(const glm::vec3& gunPosition, const glm::vec3* pivot);

	GLvoid Update();

	inline constexpr GLvoid StartFire() { mIsFire = true; }
	inline constexpr GLvoid StopFire() { mIsFire = false; }

	GLvoid Rotate(const GLfloat& yaw, const GLfloat& pitch);
};