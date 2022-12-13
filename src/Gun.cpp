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
	//mObject->SetColor(WHITE);

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


GLvoid Gun::Shot()
{
	// ----------------------------------
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	soundManager->PlayEffectSound(EffectSound::Normal_shot);
	bulletManager->Create(BulletType::Normal, WHITE, origin, bulletPos, mYaw, mPitch);

}


ShotGun::ShotGun(const glm::vec3& gunPosition, const glm::vec3* pivot) : Gun(gunPosition, pivot)
{
	mType = GunType::Blue;
	mMaxAmmo = 20;
	mAmmo = mMaxAmmo; 
	mFireDelay = 0.2f;
}

GLvoid ShotGun::Shot() 
{
	
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	bulletManager->Create(BulletType::Normal, BLUE, origin, bulletPos, mYaw, mPitch);
	soundManager->PlayEffectSound(EffectSound::Shotgun_shot);
	for (size_t i = 0; i < mBuckbullets; i++)
	{
		GLfloat m_b_angle = mBuckAngle - (i * mBuckAngle/mBuckbullets * 2);
		bulletManager->Create(BulletType::Normal, BLUE, origin, bulletPos, mYaw, mPitch + m_b_angle);
		// mPitch + m_b_angle 부터 mPitch - m_b_angle 까지 mBuckbullets 만큼 발사
	}

}

Sniper::Sniper(const glm::vec3& gunPosition, const glm::vec3* pivot) : Gun(gunPosition, pivot)
{
	mType = GunType::Green;
	mMaxAmmo = 8;
	mAmmo = mMaxAmmo;
	mFireDelay = 0.5f;
}

GLvoid Sniper::Shot()
{
	
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	soundManager->PlayEffectSound(EffectSound::Sniper_shot);
	bulletManager->Create(BulletType::Normal, GREEN, origin, bulletPos, mYaw, mPitch);

}

Launcher::Launcher(const glm::vec3& gunPosition, const glm::vec3* pivot) : Gun(gunPosition, pivot)
{
	mType = GunType::Red;
	mMaxAmmo = 5;
	mAmmo = mMaxAmmo;
	mFireDelay = 0.4f;
}

GLvoid Launcher::Shot()
{
	
	// ----------------------------------
	glm::vec3 origin = { 0, 9, 0 };
	glm::vec3 bulletPos = { 0, 9, 38 };

	MultiplyVector(mObject->GetTransform(), bulletPos);
	MultiplyVector(mObject->GetTransform(), origin);
	soundManager->PlayEffectSound(EffectSound::Launcher_shot);
	bulletManager->Create(BulletType::Rocket, RED, origin, bulletPos, mYaw, mPitch);

}