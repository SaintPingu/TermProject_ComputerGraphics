#include "stdafx.h"

class Model;


////////// [ TOP ROOT ] //////////
class Object abstract {
protected:
	glm::vec3 position = { 0, 0, 0 };
	glm::vec3 scale = { 0, 0, 0 };
	glm::vec3 scaleOrigin = { 0, 0, 0 };

	glm::vec3 look = { 0, 0, 0 };
	glm::quat rotation = { 0, 0, 0, 0 };
	glm::quat localRotation = { 0, 0, 0, 0 };
	const glm::vec3* pivot = nullptr;
	const glm::vec3* rotationPivot = nullptr;
public:
	Object();
	~Object();
	virtual GLvoid InitValues();
	// scaling
	GLvoid SetScale(const GLfloat& scale);
	GLvoid SetScale(const glm::vec3& scale);
	GLvoid ScaleOrigin(const GLfloat& scale);
	GLvoid Scale(const GLfloat& scale);
	GLvoid ScaleX(const GLfloat& amount);
	GLvoid ScaleY(const GLfloat& amount);
	GLvoid ScaleZ(const GLfloat& amount);
	GLvoid SetScaleX(const GLfloat& scale);
	GLvoid SetScaleY(const GLfloat& scale);
	GLvoid SetScaleZ(const GLfloat& scale);
	const glm::vec3* GetRefScale() const;
	inline constexpr glm::vec3 GetScale() const
	{
		return scale;
	}

	// rotation
	GLvoid Rotate(const glm::vec3& axis, const GLfloat& degree);
	GLvoid RotateLocal(const glm::vec3& axis, const GLfloat& degree);
	GLvoid RotateLocal(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll);
	GLvoid RotatePivot(const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree);
	GLvoid RotatePosition(const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree);
	GLvoid SetLocalRotation(const glm::vec3& axis, const GLfloat& degree);
	GLvoid ResetRotation();
	GLvoid ResetLocalRotation();
	glm::quat GetRotation() const;
	glm::quat GetLocalRotation() const;
	GLvoid SetRotationPivot(const glm::vec3* pivot);

	// position
	const glm::vec3* GetRefPos() const;
	glm::vec3 GetPosition() const;
	GLvoid SetPosition(const glm::vec3& position);
	GLvoid SetPosX(const GLfloat& amount);
	GLvoid SetPosY(const GLfloat& amount);
	GLvoid SetPosZ(const GLfloat& amount);

	GLvoid Move(const glm::vec3& vector);
	GLvoid MoveX(const GLfloat& amount);
	GLvoid MoveY(const GLfloat& amount);
	GLvoid MoveZ(const GLfloat& amount);
	GLvoid MoveGlobal(const glm::vec3& vector);
	GLvoid SetPivot(const glm::vec3* pivot);

	// direction
	GLvoid SetLook(const glm::vec3& look);
	glm::vec3 GetLook() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;

	// transform
	glm::mat4 GetTransform() const;
	GLvoid ModelTransform() const;

	// draw
	virtual GLvoid Draw() const abstract;
};



////////// [ MAJOR ROOT ] //////////
class IdentityObject : public Object{
protected:
	GLuint VAO = 0;
	GLuint VBO[2] = { 0,0 };
	GLuint EBO = 0;

	virtual GLvoid PullColors(vector<GLfloat>& colors) const abstract;
	virtual GLvoid PullVertices(vector<GLfloat>& vertices) const abstract;
	virtual GLvoid PullIndices(vector<size_t>& vertexIndices) const abstract;

	GLvoid InitValues();
public:
	~IdentityObject();
	virtual GLvoid InitBuffers();
	virtual GLvoid BindBuffers();

	virtual GLfloat GetWidth() const;
	virtual GLfloat GetHeight() const;
	virtual GLfloat GetDepth() const;

	// buffers
	virtual size_t GetIndexCount() const abstract;
	virtual size_t GetVertexCount() const abstract;
	inline constexpr const GLuint& GetVAO() const
	{
		return VAO;
	}

	// draw
	virtual GLvoid Draw() const;
};

class SharedObject : public Object {
protected:
	const IdentityObject* object = nullptr;
	MyColor color;
	GLboolean isChangeColor = false;
public:
	SharedObject() {}
	SharedObject(const IdentityObject* object);
	inline constexpr GLvoid SetObject(const IdentityObject* object)
	{
		this->object = object;
	}

	GLvoid Draw() const;

	GLvoid SetColor(const COLORREF& color);
	GLvoid DisableColor();
	inline constexpr const IdentityObject* GetIdentityObject() const
	{
		return object;
	}
};





////////// [ MINOR ROOTS ] //////////
class ModelObject : public IdentityObject {
protected:
	const Model* model = nullptr;
	vector<MyColor> colors;

	GLvoid PullColors(vector<GLfloat>& colors) const;
	GLvoid PullVertices(vector<GLfloat>& vertices) const;
	GLvoid PullIndices(vector<size_t>& vertexIndices) const;
public:
	ModelObject() {};
	ModelObject(const Model* model);
	GLvoid LoadModel(const Model* model);

	// buffers
	size_t GetIndexCount() const;
	size_t GetVertexCount() const;

	// size
	GLfloat GetSide(const Dir& dir) const;
	GLfloat GetWidth() const;
	GLfloat GetHeight() const;
	GLfloat GetDepth() const;

	// colors
	GLvoid ClearColor();
	GLvoid SetColor(const COLORREF& color);
	GLvoid SetColor(const size_t& index, const MyColor& color);
	GLvoid RandomizeColor();

	// collision
	GLrect GetXZRect() const;
};

class CustomObject : public IdentityObject {
protected:
	vector<glm::vec3> vertices;
	vector<size_t> indices;
	vector<MyColor> colors;

	GLvoid PullColors(vector<GLfloat>& colors) const;
	GLvoid PullVertices(vector<GLfloat>& vertices) const;
	GLvoid PullIndices(vector<size_t>& vertexIndices) const;
public:
	CustomObject();
	CustomObject(vector<glm::vec3>& vertices);
	CustomObject(vector<glm::vec3>& vertices, vector<size_t>& indices);

	// buffers
	size_t GetIndexCount() const;
	size_t GetVertexCount() const;

	// colors
	GLvoid SetColor(const COLORREF& color);
	GLvoid SetColor(const size_t& index, const COLORREF& color);
};






////////// [ BASIC OBJECTS ] //////////

class Line : public CustomObject {
public:
	Line();
	Line(const glm::vec3& v1, const glm::vec3& v2);
	GLvoid Draw() const;
};
class Triangle : public CustomObject {
public:
	Triangle();
	Triangle(vector<glm::vec3>& vertices) : CustomObject(vertices) { if (vertices.size() != 3) assert(0); }
	GLvoid Draw() const;
};
class Plane : public CustomObject {
public:
	Plane();
	GLvoid Draw() const;
	glm::vec3 GetNormal() const;
	GLfloat CheckCollision(const glm::vec3& point, const GLfloat& radius) const;
};


class Cube : public ModelObject {
private:
	vector<Cube*> childs;
	vector<glm::vec3> pivots;
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

class Ball : public ModelObject {
private:
	GLfloat speed = 1.0f;
	set<const Plane*> interPlanes;
public:
	glm::vec3 vector = { 0,0,0 };
	Ball();
	GLvoid Update();

	GLvoid CheckCollision();
};


class Cuboid {
private:
	const glm::vec3* position = nullptr;
	const glm::vec3* scale = nullptr;
	GLfloat halfWidth = 0.0f;
	GLfloat height = 0.0f;
	GLfloat halfDepth = 0.0f;
public:
	Cuboid(const IdentityObject* object);
	Cuboid(const SharedObject* object, const GLfloat& width, const GLfloat& height, const GLfloat& depth);
	Cuboid(const glm::vec3* position, const glm::vec3* scale, const GLfloat& width, const GLfloat& height, const GLfloat& depth);

	GLfloat GetSide(const Dir& dir) const;
	GLboolean CheckCollide(const glm::vec3& center, const GLfloat& radius = 0.0f) const;

	// xz plane
	GLrect GetXZRect() const;
	GLboolean CheckCollide(const GLrect& rect) const;
	GLvoid Draw() const;
};


////////// [ OBJECTS ] //////////












const Cube* GetIdentityCube();
const Line* GetIdentityLine();
const ModelObject* GetIdentityPlayer();