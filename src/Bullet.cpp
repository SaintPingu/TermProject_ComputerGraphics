#include "stdafx.h"
#include "Bullet.h"
#include "Timer.h"
#include "Sound.h"

#define BULLET_RADIUS 10.0f

extern SoundManager* soundManager;
extern BulletManager* bulletManager;

GLvoid IBulletCollisionable::Destroy()
{
	extern BulletManager* bulletManager;
	mIsDestroyed = GL_TRUE;
	bulletManager->DelCollisionObject(this);
};


BulletManager::Bullet::Bullet(const BulletType& type, const COLORREF& color, const glm::vec3& origin, const glm::vec3& position, const GLfloat& yaw, const GLfloat& pitch) : SharedObject()
{
	mType = type;

	GLfloat scale = 0.0f;
	switch (type)
	{
	case BulletType::Normal:
		mWeight = 30.0f;
		scale = 0.1f;
		mVelocity = 300.0f;
		mDamage = 20.0f;
		SharedObject::Init(GetIdentityModelObject(Models::LowSphere));
		break;
	case BulletType::Particle_Explosion:
		mWeight = 100.0f;
		scale = 0.1f;
		mVelocity = 150.0f;
		mDamage = 0.0f;
		SharedObject::Init(GetIdentityModelObject(Models::LowSphere));
		break;
	case BulletType::Rocket:
		mWeight = 100.0f;
		scale = 1.5f;
		mVelocity = 300.0f;
		mDamage = 60.0f;
		SharedObject::Init(GetIdentityModelObject(Models::GeoSphere));
		break;

	case BulletType::Sniper:
		mWeight = 10.0f;
		scale = 0.1f;
		mVelocity = 500.0f;
		mDamage = 150.0f;
		SharedObject::Init(GetIdentityModelObject(Models::LowSphere));
		break;

	default:
		assert(0);
		break;
	}

	GLfloat resultYaw = yaw + rand() % (mSpreadAmount*2) - mSpreadAmount;
	GLfloat resultPitch = pitch + rand() % (mSpreadAmount*2) - mSpreadAmount;

	SetScale(scale);
	RotateLocal(0, resultPitch, 0);
	SetColor(color);

	mPrevPos = origin;
	mPosition = position;
	mPosition.x += rand() % 2 - 1;
	mPosition.y += rand() % 2 - 1;

	mAngleY = sin(DEGREE_TO_RADIAN(resultYaw));
	mAngleZ = cos(DEGREE_TO_RADIAN(resultYaw));

}
BulletManager::Bullet::~Bullet()
{
	
}

GLvoid BulletManager::Bullet::Update()
{
	mPrevPos = GetTransformedPos();

	/* https://www.101computing.net/projectile-motion-formula/ */
	mT += timer::DeltaTime();
	MoveZ(mVelocity * mAngleZ);
	MoveY(mVelocity * mAngleY - (0.5f * GRAVITY * mT * mT * mWeight));
}
BulletAtt BulletManager::Bullet::GetAttribute() const
{
	BulletAtt result;
	result.prevPos = mPrevPos;
	result.crntPos = GetTransformedPos();
	result.radius = BULLET_RADIUS * GetScale().x;
	result.damage = mDamage;

	return result;
}
COLORREF BulletManager::Bullet::GetColor() const
{
	return ShaderObject::GetColor();
}
















BulletManager::BulletManager()
{
	mBulletList.reserve(1000);
}
BulletManager::~BulletManager()
{
	for (Bullet* bullet : mBulletList)
	{
		delete bullet;
	}
}

GLvoid BulletManager::Create(const BulletType& type, const COLORREF& color, const glm::vec3& origin, const glm::vec3& position, const GLfloat& yaw, const GLfloat& pitch)
{
	Bullet* bullet = new Bullet(type, color, origin, position, yaw, pitch);
	mBulletList.emplace_back(bullet);
}
GLvoid BulletManager::CreateExplosion(const COLORREF& color, const glm::vec3& position, const GLfloat& radius, const GLint& amount)
{
	glm::vec3 origin = position;
	const GLint r = static_cast<GLint>(radius);

	for (GLint i = 0; i < amount; ++i)
	{
		GLfloat yaw = static_cast<GLfloat>(rand() % 180) - 90.0f;
		GLfloat pitch = static_cast<GLfloat>(rand() % 360);
		glm::vec3 pos = position;

		
		pos.x += rand() % (r * 2) - r;
		pos.y += rand() % (r * 2) - r;
		pos.z += rand() % (r * 2) - r;

		Create(BulletType::Particle_Explosion, color, origin, pos, yaw, pitch);
	}
}
GLvoid BulletManager::Draw() const
{
	for (const Bullet* bullet : mBulletList)
	{
		bullet->Draw();
	}
}
GLvoid BulletManager::Update()
{
	constexpr GLfloat NO_NORMAL = 9;

	for(auto iter = mBulletList.begin(); iter != mBulletList.end();)
	{
		Bullet* bullet = (*iter);

		for (IBulletCollisionable* object : mCollisionObjectList)
		{
			glm::vec3 hitPoint;
			glm::vec3 normal = { NO_NORMAL, NO_NORMAL, NO_NORMAL };

			if (object->CheckCollisionBullet(bullet->GetAttribute(), hitPoint, normal) == GL_TRUE)
			{
				/* create paint */
				if (normal.x != NO_NORMAL)
				{
					GLuint randPaint = rand() % NUM_PAINT;
					Textures texture = static_cast<Textures>(static_cast<GLuint>(Textures::Paint) + randPaint);
					const IdentityObject* object = GetIdentityTextureObject(texture);
					if (bullet->GetType() != BulletType::Rocket)
					{
						soundManager->PlayEffectSound(EffectSound::Drawing_ink);
					}
					else
					{
						soundManager->PlayEffectSound(EffectSound::Drawing_Bigink);
					}
					PaintPlane* plane = new PaintPlane(object, bullet->GetColor(), hitPoint, normal);
					plane->SetScale(BULLET_RADIUS * bullet->GetScale());
					mPaints.emplace_back(plane);
				}

				if (bullet->GetType() == BulletType::Rocket)
				{
					bulletManager->CreateExplosion(RED, bullet->GetCenterPos(), bullet->GetRadius());
				}

				delete bullet;
				bullet = nullptr;
				iter = mBulletList.erase(iter);
				break;
			}
		}

		/* didn't collision */
		if (bullet != nullptr)
		{
			bullet->Update();
			++iter;
		}
	}

	for (auto iter = mPaints.begin(); iter != mPaints.end();)
	{
		PaintPlane* paint = *iter;
		if (paint->Update() == GL_FALSE)
		{
			delete paint;
			iter = mPaints.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

GLvoid BulletManager::AddCollisionObject(IBulletCollisionable* object)
{
	mCollisionObjectList.emplace_back(object);
	object->SetID(mID++);
}
GLvoid BulletManager::DelCollisionObject(IBulletCollisionable* object) {
	mCollisionObjectList.erase(remove_if(mCollisionObjectList.begin(), mCollisionObjectList.end() - 1, [&object](IBulletCollisionable* item) {return object->GetID() == item->GetID(); }));
};