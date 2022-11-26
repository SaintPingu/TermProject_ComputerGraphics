#pragma once
#include "stdafx.h"
#include "Object.h"

class BulletManager {
private:
	class Bullet : public SharedObject {
		/* 무게, 중력의 영향을 받는 정도 (n배) */
		const GLfloat mWeight = 30.0f;
		const GLfloat mRadius = 0.1f;

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
		Bullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch);
		GLvoid Update();

		inline constexpr GLfloat GetRadius() const { return mRadius; }
		inline constexpr glm::vec3 GetPrevPos() const { return mPrevPos; }
	};

	vector<Bullet*> mBulletList;
	vector<IBulletCollisionable*> mCollisionObjectList;
public:
	BulletManager();
	~BulletManager();
	GLvoid AddBullet(const glm::vec3& position, const GLfloat& velocity, const GLfloat& yaw, const GLfloat& pitch);
	GLvoid Draw() const;
	GLvoid Update();

	inline GLvoid AddCollisionObject(IBulletCollisionable* object) { mCollisionObjectList.emplace_back(object); }
};