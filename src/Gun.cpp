#include "stdafx.h"
#include "Gun.h"
#include "Object.h"
#include "Bullet.h"
#include "Timer.h"
#include "Sound.h"

extern SoundManager* soundManager;
extern BulletManager* bulletManager;

Gun::Gun(const glm::vec3& gunPosition, const glm::vec3* pivot)
{
	mObject = new SharedObject(GetIdentityTextureObject(Textures::Gun));

	mObject->SetPivot(pivot);
	mObject->SetPosition(gunPosition);

	mGunPosition = gunPosition;
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

	if (mAmmo > 0)
	{
		--mAmmo;
	}
	else
	{
		//cout << "총알 부족" << endl;
		mAmmo = mMaxAmmo;
	}
	 

	mCrntJumpDelay = 0.0f;
	Shot();
}

GLvoid Gun::Draw()
{
	mObject->Draw();
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
GLvoid Gun::RotateLocal(const GLfloat& yaw, const GLfloat& pitch)
{
	mYaw = yaw;
	mPitch = pitch;

	mObject->ResetRotation();
	mObject->ResetLook();
	mObject->SetPosition(mGunPosition);
	mObject->RotatePosition({ mGunPosition.x , mGunPosition.y + 10, 0 }, Vector3::Right(), yaw);

	mObject->Rotate(Vector3::Up(), mPitch);
	mObject->RotateLocal(mYaw, 0, 0);
}

GLvoid Gun::Shot()
{
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);

	switch (mType)
	{
	case GunType::Rifle:
		bulletManager->Create(BulletType::Normal, WHITE, origin, bulletPos, mYaw, mPitch, mVelocity);
		soundManager->PlayEffectSound(EffectSound::Normal_shot, 0.1f, GL_TRUE);
		break;
	case GunType::Shotgun:
		bulletManager->Create(BulletType::Normal, BLUE, origin, bulletPos, mYaw, mPitch, mVelocity);
		soundManager->PlayEffectSound(EffectSound::Shotgun_shot, 0.2f, GL_TRUE);
		break;
	case GunType::Launcher:
		bulletManager->Create(BulletType::Rocket, RED, origin, bulletPos, mYaw, mPitch, mVelocity);
		soundManager->PlayEffectSound(EffectSound::Launcher_shot, 0.2f, GL_TRUE);
		break;
	case GunType::Sniper:
		bulletManager->Create(BulletType::Normal, GREEN, origin, bulletPos, mYaw, mPitch, mVelocity);
		soundManager->PlayEffectSound(EffectSound::Sniper_shot, 0.1f, GL_TRUE);
		break;
	}
}

Rifle::Rifle(const glm::vec3& gunPosition, const glm::vec3* pivot) : Gun(gunPosition, pivot)
{
	mType = GunType::Rifle;
	mMaxAmmo = 30;
	mAmmo = mMaxAmmo;
	mFireDelay = 0.1f;
	mVelocity = 300.0f;
}

ShotGun::ShotGun(const glm::vec3& gunPosition, const glm::vec3* pivot) : Gun(gunPosition, pivot)
{
	mType = GunType::Shotgun;
	mMaxAmmo = 20;
	mAmmo = mMaxAmmo; 
	mFireDelay = 0.2f;
	mVelocity = 200.0f;
}

GLvoid ShotGun::Shot() 
{
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	bulletManager->Create(BulletType::Normal, BLUE, origin, bulletPos, mYaw, mPitch, mVelocity);
	for (GLint i = 0; i < mBuckbullets; i++)
	{
		GLfloat m_b_angle = mBuckAngle - (i * mBuckAngle/mBuckbullets * 2);
		bulletManager->Create(BulletType::Normal, BLUE, origin, bulletPos, mYaw, mPitch + m_b_angle, mVelocity);
		// mPitch + m_b_angle 부터 mPitch - m_b_angle 까지 mBuckbullets 만큼 발사
	}

	soundManager->PlayEffectSound(EffectSound::Shotgun_shot, 0.2f, GL_TRUE);
}

Sniper::Sniper(const glm::vec3& gunPosition, const glm::vec3* pivot) : Gun(gunPosition, pivot)
{
	mType = GunType::Sniper;
	mMaxAmmo = 8;
	mAmmo = mMaxAmmo;
	mFireDelay = 0.5f;
	mVelocity = 500.0f;
}

Launcher::Launcher(const glm::vec3& gunPosition, const glm::vec3* pivot) : Gun(gunPosition, pivot)
{
	mType = GunType::Launcher;
	mMaxAmmo = 5;
	mAmmo = mMaxAmmo;
	mFireDelay = 0.4f;
	mVelocity = 150.0f;
}