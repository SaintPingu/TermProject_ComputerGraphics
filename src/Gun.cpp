#include "stdafx.h"
#include "Gun.h"
#include "Object.h"
#include "Bullet.h"

Gun::Gun(const glm::vec3& gunPosition, const glm::vec3* pivot)
{
	mObject = new SharedObject(GetIdentityObject(IdentityObjects::Gun));

	mObject->SetColor(YELLOW);
	mObject->SetPivot(pivot);
	mObject->SetPosition(gunPosition);

	mGunPosition = gunPosition;
}

GLvoid Gun::Draw() const
{
	mObject->Draw();
}
GLvoid Gun::Fire(const GLfloat& yaw, const GLfloat& pitch) const
{
	extern BulletManager* bulletManager;
	glm::vec3 bulletPos = { 0, 9, 38 };
	MultiplyVector(mObject->GetTransform(), bulletPos);
	bulletManager->AddBullet(bulletPos, 200.0f, yaw, pitch);
}
GLvoid Gun::Rotate(const GLfloat& yaw, const GLfloat& pitch) const
{
	mObject->ResetRotation();
	mObject->ResetLook();
	mObject->SetPosition(mGunPosition);
	mObject->RotatePosition({ mGunPosition.x , mGunPosition.y + 20 , 0 }, Vector3::Right(), -yaw);

	mObject->Rotate(Vector3::Up(), pitch);
	mObject->RotateLocal(-yaw, 0, 0);
}