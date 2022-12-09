#include "stdafx.h"
#include "Gun.h"
#include "Object.h"
#include "Bullet.h"
#include "Timer.h"
#include "Sound.h"

extern SoundManager* soundManager;

Gun::Gun(const glm::vec3& gunPosition, const glm::vec3* pivot)
{
	mObject = new SharedObject(GetIdentityTextureObject(Textures::Gun));

	mObject->SetPivot(pivot);
	mObject->SetPosition(gunPosition);

	mGunPosition = gunPosition;
	AddObject(Shader::Texture, mObject);
}

GLvoid Gun::Update()
{
	if (mIsFire == GL_FALSE)
	{
		return;
	}

	mCrntJumpDelay += timer::DeltaTime();
	if (mCrntJumpDelay < mFireDelay)
	{
		return;
	}

	if (mAmmo > 0) // shot
	{
		soundManager->PlayEffectSound(EffectSound::Normal_shot);
		mAmmo--;
	}

	else {
		//cout << "ÃÑ¾Ë ºÎÁ·" << endl;
		mAmmo = mMaxAmmo;
	}


	mCrntJumpDelay = 0.0f;

	// ----------------------------------
	extern BulletManager* bulletManager;
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	bulletManager->Create(BulletType::Normal, RED, origin, bulletPos, mYaw, mPitch);
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
