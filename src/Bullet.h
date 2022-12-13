#pragma once
#include "stdafx.h"
#include "Object.h"

enum class BulletType { Normal, Particle_Explosion, Rocket, Sniper};

typedef struct BulletAtt {
	glm::vec3 prevPos = glm::vec3(0, 0, 0);
	glm::vec3 crntPos = glm::vec3(0, 0, 0);
	GLfloat radius = 0.0f;
	GLfloat damage = 0.0f;
}BulletAtt;

class IBulletCollisionable abstract {
private:
	GLint mID = 0;
	GLboolean mIsDestroyed = GL_FALSE;
public:
	virtual GLboolean CheckCollisionBullet(const BulletAtt& bullet, glm::vec3& hitPoint, glm::vec3& normal) abstract;
	GLvoid Destroy();
	inline constexpr GLboolean IsDestroyed() const { return mIsDestroyed; };
	inline constexpr GLvoid SetID(const GLint& id) { mID = id; }
	inline constexpr GLint GetID() { return mID; }
};



class BulletManager {
private:
	class Bullet : public SharedObject {
		BulletType mType = BulletType::Normal;

		/* ����, �߷��� ������ �޴� ���� (n��) */
		GLfloat mWeight = 0.0f;

		GLfloat mDamage = 0.0f;

		/* ���� ��ǥ�� */
		glm::vec3 mPrevPos = glm::vec3(0, 0, 0);
		/* ���� ��ǥ�� */
		glm::vec3 mOrigin = glm::vec3(0, 0, 0);

		/* �߻� �� ����� �ð� */
		GLfloat mT = 0.0f;

		/* ź���� */
		GLint mSpreadAmount = 2;

		GLfloat mAngleX = 0.0f;
		GLfloat mAngleY = 0.0f;
		GLfloat mAngleZ = 0.0f;
		GLfloat mVelocity = 0.0f;
	public:
		Bullet(const BulletType& type, const COLORREF& color, const glm::vec3& origin, const glm::vec3& position, const GLfloat& yaw, const GLfloat& pitch);
		~Bullet();
		GLvoid Update();

		BulletAtt GetAttribute() const;
		COLORREF GetColor() const;
		inline constexpr BulletType GetType() const { return mType; }
	};

	GLint mID = 0;
	vector<Bullet*> mBulletList;
	vector<PaintPlane*> mPaints;
	vector<IBulletCollisionable*> mCollisionObjectList;
public:
	BulletManager();
	~BulletManager();
	GLvoid Create(const BulletType& type, const COLORREF& color, const glm::vec3& origin, const glm::vec3& position, const GLfloat& yaw, const GLfloat& pitch);
	GLvoid CreateExplosion(const COLORREF& color, const glm::vec3& position, const GLfloat& radius, const GLint& amount = 20);
	GLvoid Draw() const;
	GLvoid Update();

	/* Read only */
	inline constexpr const vector<PaintPlane*>& GetPaints() const { return mPaints; }

	GLvoid AddCollisionObject(IBulletCollisionable* object);
	GLvoid DelCollisionObject(IBulletCollisionable* object);
};