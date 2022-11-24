#pragma once
#include "Object.h"

enum class GunType { None=0, Red, Blue, Green, White };

class Gun {
private:
	GunType mType = GunType::None;
	SharedObject* mObject = nullptr;
	glm::vec3 mGunPosition = { 0,0,0 };
public:

	Gun(const glm::vec3& gunPosition, const glm::vec3* pivot);
	GLvoid Draw() const;

	GLvoid Fire(const GLfloat& yaw, const GLfloat& pitch) const;
	GLvoid Rotate(const GLfloat& yaw, const GLfloat& pitch) const;
};