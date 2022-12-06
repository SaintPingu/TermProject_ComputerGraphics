#pragma once
#include "stdafx.h"
#include "Object.h"

enum class BulletType { Normal };

typedef struct BulletAtt {
	glm::vec3 prevPos = glm::vec3(0, 0, 0);
	glm::vec3 crntPos = glm::vec3(0, 0, 0);
	GLfloat radius = 0.0f;
	GLfloat damage = 0.0f;
}BulletAtt;

class IBulletCollisionable abstract {
private:
	GLint mID = 0;
	GLboolean isDestroyed = GL_FALSE;
public:
	virtual GLboolean CheckCollisionBullet(const BulletAtt& bullet, glm::vec3& hitPoint, glm::vec3& normal) abstract;
	GLvoid Destroy();
	inline constexpr GLboolean IsDestroyed() const { return isDestroyed; };
	inline constexpr GLvoid SetID(const GLint& id) { mID = id; }
	inline constexpr GLint GetID() { return mID; }
};



class BulletManager {
private:
	class Bullet : public SharedObject {
		/* 무게, 중력의 영향을 받는 정도 (n배) */
		GLfloat mWeight = 0.0f;

		GLfloat mRadius = 0.0f;
		GLfloat mDamage = 0.0f;

		/* 이전 좌표값 */
		glm::vec3 mPrevPos = { 0,0,0 };

		/* 발사 후 경과한 시간 */
		GLfloat mT = 0.0f;

		/* 탄퍼짐 */
		GLint mSpreadAmount = 2;

		GLfloat mAngleY = 0.0f;
		GLfloat mAngleZ = 0.0f;
		GLfloat mVelocity = 0.0f;
	public:
		Bullet(const BulletType& type, const glm::vec3& origin, const glm::vec3& position, const GLfloat& yaw, const GLfloat& pitch);
		GLvoid Update();

		BulletAtt GetAttribute() const;
	};

	vector<Bullet*> mBulletList;
	vector<PaintPlane*> mPaints;
	vector<IBulletCollisionable*> mCollisionObjectList;
public:
	BulletManager();
	~BulletManager();
	GLvoid Create(const BulletType& type, const glm::vec3& origin, const glm::vec3& position, const GLfloat& yaw, const GLfloat& pitch);
	GLvoid Draw() const;
	GLvoid Update();

	/* Read only */
	inline constexpr const vector<PaintPlane*>& GetPaints() const { return mPaints; }

	inline GLvoid AddCollisionObject(IBulletCollisionable* object) { mCollisionObjectList.emplace_back(object); }
	inline GLvoid DelCollisionObject(IBulletCollisionable* object) {
		mCollisionObjectList.erase(remove_if(mCollisionObjectList.begin(), mCollisionObjectList.end(), [&object](IBulletCollisionable* item) {return object->GetID() == item->GetID(); }));
	};
};