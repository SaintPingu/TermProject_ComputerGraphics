#pragma once
#include "stdafx.h"
#include "Model.h"

#define DEFAULT_OBJECT_COLOR GRAY

enum class Shader;

/************************************************** [ TOP ROOTS ] **************************************************/

//////////////////////////////////////// [ Object ] ////////////////////////////////////////
class Object abstract {
protected:
	/* Rotation, Scaling, Pivot 이전 원래 위치 */
	glm::vec3 mPosition = { 0, 0, 0 };

	/* 자전, 바라보는 방향 */
	glm::vec3 mLook = { 0, 0, 0 };
	/* 공전, look 에 영향 X */
	glm::quat mRotation = { 0, 0, 0, 0 };

	/* rotation(공전) 중심 좌표 */
	const glm::vec3* mRotationPivot = nullptr;
	/* position 중심 좌표 */
	const glm::vec3* mPivot = nullptr;

public:
	Object();
	/* 멤버변수 초기화 */
	virtual GLvoid InitValues();

	//********** [ Position ] **********//
	/* mPosition의 const 주소 리턴 */
	const glm::vec3* GetRefPos() const;

	/* mPosition 리턴 */
	inline constexpr glm::vec3 GetPosition() const { return mPosition; }
	/* pivot이 적용된 mPosition 리턴 */
	glm::vec3 GetPviotedPosition() const;

	GLvoid SetPosition(const glm::vec3& position);
	GLvoid SetPosX(const GLfloat& x);
	GLvoid SetPosY(const GLfloat& y);
	GLvoid SetPosZ(const GLfloat& z);

	GLvoid Move(const glm::vec3& vector, const GLboolean& applyTime = true);
	GLvoid MoveX(const GLfloat& amount, const GLboolean& applyTime = true);
	GLvoid MoveY(const GLfloat& amount, const GLboolean& applyTime = true);
	GLvoid MoveZ(const GLfloat& amount, const GLboolean& applyTime = true);
	/* look벡터 영향 없는 글로벌 좌표계에서 이동 */
	GLvoid MoveGlobal(const glm::vec3& vector, const GLboolean& applyTime = true);
	GLvoid SetPivot(const glm::vec3* pivot);

	//********** [ Rotation ] **********//
	/* axis를 기준으로 공전 */
	GLvoid Rotate(const glm::vec3& axis, const GLfloat& degree);
	/* yaw(x축), pitch(y축), roll(z축) 에 대해 공전 */
	GLvoid Rotate(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll);
	/* mRotation을 axis를 기준으로 회전하는 quaternion값으로 설정 */
	GLvoid SetRotation(const glm::vec3& axis, const GLfloat& degree);
	GLvoid SetRotation(const glm::quat& rotation);

	/* axis를 기준으로 자전, axis는 normal */
	GLvoid RotateLocal(const glm::vec3& axis, const GLfloat& degree);
	/* yaw(x축), pitch(y축), roll(z축) 에 대해 자전 */
	GLvoid RotateLocal(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll);
	/* mLocalRotation을 axis를 기준으로 회전하는 quaternion값으로 설정 */
	GLvoid SetLocalRotation(const glm::vec3& axis, const GLfloat& degree);

	/* pivot 위치에 대해 axis를 기준으로 공전 */
	GLvoid RotatePivot(const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree);
	/* pivot 위치에 대해 axis를 기준으로 위치만 공전 */
	GLvoid RotatePosition(const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree);
	
	GLvoid ResetRotation();
	glm::quat GetRotation() const;
	GLvoid SetRotationPivot(const glm::vec3* pivot);

	//********** [ Direction ] **********//
	/* look을 point를 바라보도록 설정 */
	GLvoid Look(const glm::vec3& point);
	GLvoid SetLook(const glm::vec3& look);
	GLvoid ResetLook();

	glm::vec3 GetLook() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;
};

//////////////////////////////////////// [ SharedObject ] ////////////////////////////////////////
class ShaderObject abstract : public Object {
protected:
	/* 사용하는 shader */
	Shader mShader = Shader::None;
	/* Object Color */
	MyColor mColor;

	/* Object 기준 Scale값 */
	glm::vec3 mScale = { 0, 0, 0 };
	/* 원점 기준 Scale값, 현재 사용 X */
	glm::vec3 mScaleOrigin = { 0, 0, 0 };

	virtual GLvoid PrepareDraw() const;
public:
	ShaderObject();
	~ShaderObject();
	/* 멤버변수 초기화 */
	virtual GLvoid InitValues() override;

	//********** [ Scaling ] **********//
	GLvoid SetScale(const GLfloat& scale);
	GLvoid SetScaleX(const GLfloat& scale);
	GLvoid SetScaleY(const GLfloat& scale);
	GLvoid SetScaleZ(const GLfloat& scale);
	GLvoid SetScale(const glm::vec3& scale);

	GLvoid ScaleOrigin(const GLfloat& scale);

	GLvoid Scale(const GLfloat& scale);
	GLvoid ScaleX(const GLfloat& amount);
	GLvoid ScaleY(const GLfloat& amount);
	GLvoid ScaleZ(const GLfloat& amount);

	inline constexpr const glm::vec3* GetRefScale() const { return &mScale; };
	inline constexpr glm::vec3 GetScale() const { return mScale; }

	//********** [ Transform ] **********//
	/* Object의 최종 Transform 리턴 */
	glm::mat4 GetTransform() const;
	/* Object의 최종 Transform 적용 */
	GLvoid ModelTransform() const;

	/* Transform이 적용된 mPosition 리턴 */
	inline glm::vec3 GetTransformedPos() const{ return glm::vec3(GetTransform() * glm::vec4(0, 0, 0, 1.0f)); }

	//********** [ Draw ] **********//
	virtual GLvoid Draw() const abstract;
	inline GLvoid SetColor(const COLORREF& color) { mColor = MyColor(color); }

	//********** [ Shader ] **********//
	inline constexpr Shader GetShader() const { return mShader; }
	inline constexpr GLvoid SetShader(const Shader& shader) { mShader = shader; }
};










/************************************************** [ MAJOR ROOTS ] **************************************************/

//////////////////////////////////////// [ IdentityObject ] ////////////////////////////////////////
// IdentityObject should BindBuffers() when generated
class IdentityObject abstract : public ShaderObject{
protected:
	GLuint mVAO = 0;
	GLuint mVBO[3] = { 0,0,0 };
	GLuint mEBO = 0;
	GLuint mTexture = 0;

	/* BindBuffers()를 위한 함수들 */
	virtual GLvoid PullVertices(vector<GLfloat>& vertices) const abstract;
	virtual GLvoid Pull_Indices_Vertex(vector<size_t>& indices_vertex) const abstract;
	virtual GLvoid PullColors(vector<GLfloat>& colors) const abstract;
	virtual GLvoid PullNormals(vector<GLfloat>& normals) const abstract;
	virtual GLvoid Pull_Indices_Normal(vector<size_t>& indices_normal) const abstract;
	virtual GLvoid PullUVs(vector<GLfloat>& uvs) const abstract;
	virtual GLvoid Pull_Indices_UV(vector<size_t>& indices_uv) const abstract;

	/* 멤버 변수 초기화 */
	virtual GLvoid InitValues() override;
public:
	IdentityObject();
	~IdentityObject();
	GLvoid InitBuffers();
	GLvoid BindBuffers();
	GLvoid DeleteBuffers();

	// 현재 사용 X //
	GLfloat GetWidth() const;
	GLfloat GetHeight() const;
	GLfloat GetDepth() const;
	/////////////////

	//********** [ Buffer ] **********//
	/* Vertex Index 개수 리턴 */
	virtual size_t GetIndexCount() const abstract;
	/* Vertex 개수 리턴 */
	virtual size_t GetVertexCount() const abstract;

	inline constexpr const GLuint& GetVAO() const { return mVAO; }
	inline MyColor GetColor() const { return mColor; }

	/* draw */
	virtual GLvoid Draw() const override;
};

//////////////////////////////////////// [ SharedObject ] ////////////////////////////////////////
class SharedObject : public ShaderObject {
protected:
	const IdentityObject* mObject = nullptr;
	GLboolean mIsChangeColor = false;
public:
	SharedObject() {};
	SharedObject(const IdentityObject* object);

	GLvoid Draw() const override;

	inline constexpr GLvoid SetObject(const IdentityObject* object) { mObject = object; }
	inline constexpr const IdentityObject* GetObject() const { return mObject; }
};











/************************************************** [ MINOR ROOTS ] **************************************************/

//////////////////////////////////////// [ ModelObject ] ////////////////////////////////////////
class ModelObject : public IdentityObject {
protected:
	const Model* mModel = nullptr;

	/* ModelObject에서는 사용하지 않음 */
	GLvoid PullColors(vector<GLfloat>& colors) const override {};

	GLvoid PullNormals(vector<GLfloat>& normals) const override;
	GLvoid PullVertices(vector<GLfloat>& vertices) const override;
	GLvoid Pull_Indices_Vertex(vector<size_t>& indices_vertex) const override;
	GLvoid Pull_Indices_Normal(vector<size_t>& indices_normal) const override;
	GLvoid PullUVs(vector<GLfloat>& textures) const override;
	GLvoid Pull_Indices_UV(vector<size_t>& indices_uv) const override;
public:
	ModelObject();
	ModelObject(const Model* model, const Shader& shader);
	GLvoid LoadModel(const Model* model, const Shader& shader);

	//********** [ Buffer ] **********//
	size_t GetIndexCount() const;
	size_t GetVertexCount() const;

	//********** [ Size ] **********//
	/* Side에 대한 값 리턴 (Left -> 왼쪽 경계값, Top -> 위쪽 경계값, ...) */
	GLfloat GetSide(const Dir& dir) const;
	GLfloat GetWidth() const;
	GLfloat GetHeight() const;
	GLfloat GetDepth() const;

	//********** [ Collision ] **********//
	GLrect GetXZRect() const;
	set<glm::vec2, CompareSet> GetBoundings_XZ() const;
};

//////////////////////////////////////// [ CustomObject ] ////////////////////////////////////////
class CustomObject : public IdentityObject {
protected:
	vector<glm::vec3> mVertices;
	vector<size_t> mIndices;

	/* CustomObject에서는 사용하지 않음 */
	GLvoid PullNormals(vector<GLfloat>& normals) const override {};
	GLvoid Pull_Indices_Normal(vector<size_t>& indices_normal) const override {};
	GLvoid PullUVs(vector<GLfloat>& textures) const {};
	GLvoid Pull_Indices_UV(vector<size_t>& indices_uv) const {};

	GLvoid PullColors(vector<GLfloat>& colors) const override;
	GLvoid PullVertices(vector<GLfloat>& vertices) const override;
	GLvoid Pull_Indices_Vertex(vector<size_t>& indices_vertex) const override;
public:
	CustomObject();
	CustomObject(vector<glm::vec3>& vertices);
	CustomObject(vector<glm::vec3>& vertices, vector<size_t>& indices);

	//********** [ Buffer ] **********//
	size_t GetIndexCount() const;
	size_t GetVertexCount() const;
};











/************************************************** [ BASIC OBJECTS ] **************************************************/

//////////////////////////////////////// [ Line ] ////////////////////////////////////////
class Line : public CustomObject {
public:
	Line();
	Line(const glm::vec3& v1, const glm::vec3& v2);
	GLvoid Draw() const;
	GLvoid SetVertex(const GLboolean& index, const glm::vec3& pos);
};

//////////////////////////////////////// [ Triangle ] ////////////////////////////////////////
class Triangle : public CustomObject {
public:
	Triangle();
	Triangle(vector<glm::vec3>& vertices) : CustomObject(vertices) { if (vertices.size() != 3) assert(0); }
	GLvoid Draw() const;
};

//////////////////////////////////////// [ Plane ] ////////////////////////////////////////
class Plane : public CustomObject {
public:
	Plane();
	GLvoid Draw() const;
	glm::vec3 GetNormal() const;
	GLfloat CheckCollision(const glm::vec3& point, const GLfloat& radius) const;
};










/************************************************** [ COLLISION OBJECTS ] **************************************************/

//////////////////////////////////////// [ Cuboid ] ////////////////////////////////////////
class Cuboid {
private:
	const glm::vec3* mPosition = nullptr;
	const glm::vec3* mScale = nullptr;
	GLfloat mHalfWidth = 0.0f;
	GLfloat mHeight = 0.0f;
	GLfloat mHalfDepth = 0.0f;
public:
	Cuboid(const IdentityObject* object);
	Cuboid(const SharedObject* object, const GLfloat& width, const GLfloat& height, const GLfloat& depth);
	Cuboid(const glm::vec3* position, const glm::vec3* scale, const GLfloat& width, const GLfloat& height, const GLfloat& depth);

	GLvoid Draw() const;

	/* Side에 대한 값 리턴 (Left -> 왼쪽 경계값, Top -> 위쪽 경계값, ...) */
	GLfloat GetSide(const Dir& dir) const;
	/* true if collision */
	GLboolean CheckCollide(const glm::vec3& center, const GLfloat& radius = 0.0f) const;
	GLboolean CheckCollide(const GLrect& rect) const;

	GLrect GetXZRect() const;
};

//////////////////////////////////////// [ Circle ] ////////////////////////////////////////
class Circle {
private:
	SharedObject* mCircle = nullptr;

	glm::vec3 mOffset = { 0,0,0 };

	GLfloat mRadius = 0.0f;
public:
	Circle(const glm::vec3* pivot, const GLfloat& radius, const glm::vec3 offset = { 0,0,0 });

	GLvoid Draw() const;

	GLvoid SetColor(const COLORREF& color);
	glm::vec2 GetCenter() const;
	GLfloat GetRadius() const;
};

/* 총알과 충돌하는 객체 */
class IBulletCollisionable abstract {
public:
	virtual GLboolean CheckCollisionBullet(const glm::vec3& prevPos, const glm::vec3& bulletPos, const GLfloat& bulletRadius, const glm::vec3* hitPoint = nullptr) abstract;
};

/* 2D 충돌체크를 하는 객체 */
class ICollisionable_2D abstract {
private:
	CollisionType mCollisionType = CollisionType::None;

	GLint id = -1;

	const glm::vec3* mPosition = nullptr;

	GLfloat mRadius = 0.0f;
	GLfloat mWidth = 0.0f;
	GLfloat mDepth = 0.0f;
public:
	virtual GLvoid SetCollision(const glm::vec3* position, const GLfloat& radius);
	virtual GLvoid SetCollision(const glm::vec3* position, const GLfloat& width, const GLfloat& depth);

	GLfloat GetRadius() const;
	GLfloat GetWidth() const;
	GLfloat GetDepth() const;

	inline constexpr CollisionType GetCollisionType() const{ return mCollisionType; }
	inline constexpr GLint Get_ID() const { return id; }
};

class CollisionManager {
private:
	ICollisionable_2D* player = nullptr;
	vector<ICollisionable_2D*> mMonsterList;
	vector<ICollisionable_2D*> mObstacleList;
public:
	enum class Tag { Player, Monster, Obstacle };
	CollisionManager();
	~CollisionManager();

	GLvoid CheckCollisions();
	GLvoid AddObject(const Tag& tag, ICollisionable_2D* object);
	GLvoid DeleteObject(ICollisionable_2D* object);
};


/************************************************** [ OTHER OBJECTS ] **************************************************/









GLvoid InitObjects();
const Line* GetIdentityLine();
const ModelObject* GetIdentityModelObject(const Models& model);



GLvoid AddObject(const Shader& shader, ShaderObject* object);
GLvoid DeleteObject(const Shader& shader, ShaderObject* object);
GLvoid ResetObjects();
GLvoid DrawObjects(const Shader& shader);

////////// [ DEBUG ] //////////
GLvoid DrawDebugWireXZ(const set<glm::vec2, CompareSet>& vertices, GLfloat yPos, const COLORREF& color = RED, const glm::vec3* pivot = nullptr);