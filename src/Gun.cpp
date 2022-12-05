#include "stdafx.h"
#include "Gun.h"
#include "Object.h"
#include "Bullet.h"
#include "Timer.h"

Gun::Gun(const glm::vec3& gunPosition, const glm::vec3* pivot)
{
	mObject = new SharedObject(GetIdentityTextureObject(TextureModels::Gun));

	mObject->SetPivot(pivot);
	mObject->SetPosition(gunPosition);

	mGunPosition = gunPosition;
}

GLvoid Gun::Draw() const
{
	//glDisable(GL_DEPTH_TEST);
	mObject->Draw();
	//glEnable(GL_DEPTH_TEST);
}
GLvoid Gun::Update()
{
	if (mIsFire == false)
	{
		return;
	}

	mCrntDelay += timer::DeltaTime();
	if (mCrntDelay < mFireDelay)
	{
		return;
	}

	mCrntDelay = 0.0f;
	extern BulletManager* bulletManager;
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	bulletManager->Create(BulletType::Normal, origin, bulletPos, mYaw, mPitch);
}
GLvoid Gun::Rotate(const GLfloat& yaw, const GLfloat& pitch)
{
	mYaw = yaw;
	mPitch = pitch;

	mObject->ResetRotation();
	mObject->ResetLook();
	mObject->SetPosition(mGunPosition);
	mObject->RotatePosition({ mGunPosition.x , mGunPosition.y + 20 , 0 }, Vector3::Right(), yaw);

	mObject->Rotate(Vector3::Up(), mPitch);
	mObject->RotateLocal(mYaw, 0, 0);
}