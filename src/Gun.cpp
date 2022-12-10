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
	AddObject(Shader::Texture, mObject); // 이거하면 총 개수만큼 계속 그리게 됨
	// 삭제후 Draw함수 추가
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
		//cout << "총알 부족" << endl;
		mAmmo = mMaxAmmo;
	}


	mCrntJumpDelay = 0.0f;

	// ----------------------------------
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	bulletManager->Create(BulletType::Normal, WHITE, origin, bulletPos, mYaw, mPitch);
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


ShotGun::ShotGun(const glm::vec3& gunPosition, const glm::vec3* pivot) : Gun(gunPosition, pivot)
{
	mType = GunType::Blue;
	mMaxAmmo = 20;
	mAmmo = mMaxAmmo;
}

GLvoid ShotGun::Update() 
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
		//cout << "총알 부족" << endl;
		mAmmo = mMaxAmmo;
	}


	mCrntJumpDelay = 0.0f;

	// ----------------------------------
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	bulletManager->Create(BulletType::Normal, BLUE, origin, bulletPos, mYaw, mPitch);
	bulletManager->Create(BulletType::Normal, BLUE, origin, bulletPos, mYaw, mPitch + 15);
	bulletManager->Create(BulletType::Normal, BLUE, origin, bulletPos, mYaw, mPitch - 15);

}

Sniper::Sniper(const glm::vec3& gunPosition, const glm::vec3* pivot) : Gun(gunPosition, pivot)
{
	mType = GunType::Green;
	mMaxAmmo = 10;
	mAmmo = mMaxAmmo;
}

GLvoid Sniper::Update()
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
		//cout << "총알 부족" << endl;
		mAmmo = mMaxAmmo;
	}


	mCrntJumpDelay = 0.0f;

	// ----------------------------------
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	bulletManager->Create(BulletType::Normal, GREEN, origin, bulletPos, mYaw, mPitch);

}

Launcher::Launcher(const glm::vec3& gunPosition, const glm::vec3* pivot) : Gun(gunPosition, pivot)
{
	mType = GunType::Red;
	mMaxAmmo = 5;
	mAmmo = mMaxAmmo;
}

GLvoid Launcher::Update()
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
		//cout << "총알 부족" << endl;
		mAmmo = mMaxAmmo;
	}


	mCrntJumpDelay = 0.0f;

	// ----------------------------------
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	bulletManager->Create(BulletType::Normal, RED, origin, bulletPos, mYaw, mPitch);

}