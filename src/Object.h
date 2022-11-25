#pragma once
#include "stdafx.h"

#define DEFAULT_OBJECT_COLOR GRAY

class Model;
enum class Shader;
enum class IdentityObjects { Circle, Player, Gun };

/************************************************** [ TOP ROOTS ] **************************************************/
//////////////////////////////////////// [ Object ] ////////////////////////////////////////
class Object abstract {
protected:
	/* Rotation, Scaling, Pivot ���� ���� ��ġ */
	glm::vec3 mPosition = { 0, 0, 0 };

	/* ����, �ٶ󺸴� ���� */
	glm::vec3 mLook = { 0, 0, 0 };
	/* ����, look �� ���� X */
	glm::quat mRotation = { 0, 0, 0, 0 };

	/* rotation(����) �߽� ��ǥ */
	const glm::vec3* mRotationPivot = nullptr;
	/* position �߽� ��ǥ */
	const glm::vec3* mPivot = nullptr;

public:
	Object();
	/* ������� �ʱ�ȭ */
	virtual GLvoid InitValues();

	//********** [ Position ] **********//
	/* mPosition�� const �ּ� ���� */
	const glm::vec3* GetRefPos() const;

	/* pivot�� ����� mPosition ���� */
	glm::vec3 GetPosition() const;

	GLvoid SetPosition(const glm::vec3& position);
	GLvoid SetPosX(const GLfloat& x);
	GLvoid SetPosY(const GLfloat& y);
	GLvoid SetPosZ(const GLfloat& z);

	GLvoid Move(const glm::vec3& vector, const GLboolean& applyTime = true);
	GLvoid MoveX(const GLfloat& amount, const GLboolean& applyTime = true);
	GLvoid MoveY(const GLfloat& amount, const GLboolean& applyTime = true);
	GLvoid MoveZ(const GLfloat& amount, const GLboolean& applyTime = true);
	/* look���� ���� ���� �۷ι� ��ǥ�迡�� �̵� */
	GLvoid MoveGlobal(const glm::vec3& vector, const GLboolean& applyTime = true);
	GLvoid SetPivot(const glm::vec3* pivot);

	//********** [ Rotation ] **********//
	/* axis�� �������� ���� */
	GLvoid Rotate(const glm::vec3& axis, const GLfloat& degree);
	/* yaw(x��), pitch(y��), roll(z��) �� ���� ���� */
	GLvoid Rotate(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll);
	/* mRotation�� axis�� �������� ȸ���ϴ� quaternion������ ���� */
	GLvoid SetRotation(const glm::vec3& axis, const GLfloat& degree);
	GLvoid SetRotation(const glm::quat& rotation);

	/* axis�� �������� ����, axis�� normal */
	GLvoid RotateLocal(const glm::vec3& axis, const GLfloat& degree);
	/* yaw(x��), pitch(y��), roll(z��) �� ���� ���� */
	GLvoid RotateLocal(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll);
	/* mLocalRotation�� axis�� �������� ȸ���ϴ� quaternion������ ���� */
	GLvoid SetLocalRotation(const glm::vec3& axis, const GLfloat& degree);

	/* pivot ��ġ�� ���� axis�� �������� ���� */
	GLvoid RotatePivot(const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree);
	/* pivot ��ġ�� ���� axis�� �������� ��ġ�� ���� */
	GLvoid RotatePosition(const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree);
	
	GLvoid ResetRotation();
	glm::quat GetRotation() const;
	GLvoid SetRotationPivot(const glm::vec3* pivot);

	//********** [ Direction ] **********//
	/* look�� point�� �ٶ󺸵��� ���� */
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
	/* ����ϴ� shader */
	Shader mShader = Shader::None;
	/* Object Color */
	MyColor mColor;

	/* Object ���� Scale�� */
	glm::vec3 mScale = { 0, 0, 0 };
	/* ���� ���� Scale��, ���� ��� X */
	glm::vec3 mScaleOrigin = { 0, 0, 0 };

public:
	ShaderObject();
	~ShaderObject();
	/* ������� �ʱ�ȭ */
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
	/* Object�� ���� Transform ���� */
	glm::mat4 GetTransform() const;
	/* Object�� ���� Transform ���� */
	GLvoid ModelTransform() const;

	/* Transform�� ����� mPosition ���� */
	inline glm::vec3 GetTransformedPos() const { return glm::vec3(GetTransform() * glm::vec4(mPosition, 1.0f)); }

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
	GLuint mVBO[2] = { 0,0 };
	GLuint mEBO = 0;

	virtual GLvoid PullColors(vector<GLfloat>& colors) const abstract;
	virtual GLvoid PullNormals(vector<GLfloat>& normals) const abstract;
	virtual GLvoid PullVertices(vector<GLfloat>& vertices) const abstract;
	virtual GLvoid PullIndices(vector<size_t>& vertexIndices) const abstract;
	virtual GLvoid PullNormalIndices(vector<size_t>& normalIndices) const abstract;

	/* ��� ���� �ʱ�ȭ */
	virtual GLvoid InitValues() override;
public:
	IdentityObject();
	~IdentityObject();
	GLvoid InitBuffers();
	GLvoid BindBuffers();
	GLvoid DeleteBuffers();

	// ���� ��� X //
	GLfloat GetWidth() const;
	GLfloat GetHeight() const;
	GLfloat GetDepth() const;
	/////////////////

	//********** [ Buffer ] **********//
	/* Vertex Index ���� ���� */
	virtual size_t GetIndexCount() const abstract;
	/* Vertex ���� ���� */
	virtual size_t GetVertexCount() const abstract;

	inline constexpr const GLuint& GetVAO() const { return mVAO; }
	inline MyColor GetColor() const { return mColor; }

	// draw
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
	inline constexpr const IdentityObject* GetIdentityObject() const { return mObject; }
};











/************************************************** [ MINOR ROOTS ] **************************************************/
//////////////////////////////////////// [ IdentityObject ] ////////////////////////////////////////
class ModelObject : public IdentityObject {
protected:
	const Model* mModel = nullptr;

	/* ModelObject������ ������� ���� */
	GLvoid PullColors(vector<GLfloat>& colors) const override {};
	GLvoid PullNormals(vector<GLfloat>& normals) const override;
	GLvoid PullVertices(vector<GLfloat>& vertices) const override;
	GLvoid PullIndices(vector<size_t>& vertexIndices) const override;
	GLvoid PullNormalIndices(vector<size_t>& normalIndices) const override;
public:
	ModelObject() {};
	ModelObject(const Model* model);
	GLvoid LoadModel(const Model* model);

	//********** [ Buffer ] **********//
	size_t GetIndexCount() const;
	size_t GetVertexCount() const;

	//********** [ Size ] **********//
	/* Side�� ���� �� ���� (Left -> ���� ��谪, Top -> ���� ��谪, ...) */
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

	/* CustomObject������ ������� ���� */
	GLvoid PullNormals(vector<GLfloat>& normals) const override {};
	GLvoid PullColors(vector<GLfloat>& colors) const override;
	GLvoid PullVertices(vector<GLfloat>& vertices) const override;
	GLvoid PullIndices(vector<size_t>& vertexIndices) const override;
	/* CustomObject������ ������� ���� */
	GLvoid PullNormalIndices(vector<size_t>& normalIndices) const override {};
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


//////////////////////////////////////// [ Cube ] ////////////////////////////////////////
class Cube : public ModelObject {
private:
	vector<Cube*> mChilds;
	vector<glm::vec3> mPivots;
public:
	Cube();
	GLvoid SetChild(Cube* cube);
	GLvoid AddPivot(const glm::vec3& pivot);
	const glm::vec3* GetPivot(const size_t& index);

	GLvoid Draw() const;
	GLvoid Move(const glm::vec3& vector);
	GLvoid Rotate(const glm::vec3& pivot, const GLfloat& degree);
	GLvoid RotateLocal(const glm::vec3& pivot, const GLfloat& degree);

	// xz plane
	GLboolean CheckCollide(const glm::vec3& point) const;
	GLboolean CheckCollide(const GLrect& rect) const;
};


//////////////////////////////////////// [ Sphere ] ////////////////////////////////////////
class Sphere : public ModelObject {
public:
	Sphere();
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

	/* Side�� ���� �� ���� (Left -> ���� ��谪, Top -> ���� ��谪, ...) */
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




/************************************************** [ OTHER OBJECTS ] **************************************************/










const Line* GetIdentityLine();
const Cube* GetIdentityCube();
const Sphere* GetIdentitySphere();
const ModelObject* GetIdentityObject(const IdentityObjects& object);



GLvoid AddObject(const Shader& shader, ShaderObject* object);
//GLvoid DeleteObject(const Shader& shader, ShaderObject* object);
GLvoid ResetObjects();
GLvoid DrawObjects(const Shader& shader);

////////// [ DEBUG ] //////////
GLvoid DrawDebugWireXZ(const set<glm::vec2, CompareSet>& vertices, GLfloat yPos, const COLORREF& color = RED, const glm::vec3* pivot = nullptr);