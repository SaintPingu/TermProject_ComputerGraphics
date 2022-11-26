#include "stdafx.h"
#include "Object.h"
#include "Model.h"
#include "Shader.h"
#include "Transform.h"
#include "Timer.h"


Object::Object()
{
	InitValues();
}
GLvoid Object::InitValues()
{
	mPosition = Vector3::Zero();

	mLook = Vector3::Look();
	mRotation = { 0.0f, 0.0f, 1.0f, 0.0f };

	mPivot = nullptr;
	mRotationPivot = nullptr;
}
GLvoid Object::Rotate(const glm::vec3& axis, const GLfloat& degree)
{
	mRotation *= glm::angleAxis(glm::radians(degree), glm::normalize(axis));
}
GLvoid Object::Rotate(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll)
{
	if (yaw != 0.0f)
	{
		glm::vec3 axis = GetRight();
		mRotation *= glm::angleAxis(glm::radians(yaw), axis);
	}
	if (pitch != 0.0f)
	{
		glm::vec3 axis = GetUp();
		mRotation *= glm::angleAxis(glm::radians(pitch), axis);
	}
	if (roll != 0.0f)
	{
		glm::vec3 axis = GetLook();
		mRotation *= glm::angleAxis(glm::radians(roll), axis);
	}
}
GLvoid Object::SetRotation(const glm::vec3& axis, const GLfloat& degree)
{
	mRotation = glm::angleAxis(glm::radians(degree), glm::normalize(axis));
}
GLvoid Object::SetRotation(const glm::quat& rotation)
{
	mRotation = rotation;
}
GLvoid Object::RotatePivot(const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree)
{
	RotateLocal(axis, degree);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(-degree), glm::normalize(axis));
	mPosition -= pivot;
	MultiplyVector(transform, mPosition);
	mPosition += pivot;
}
GLvoid Object::RotateLocal(const glm::vec3& axis, const GLfloat& degree)
{
	::Rotate(mLook, glm::radians(degree), glm::normalize(axis));
}
GLvoid Object::RotateLocal(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll)
{
	if (pitch != 0.0f)
	{
		glm::vec3 axis = GetUp();
		::Rotate(mLook, glm::radians(pitch), axis);
	}
	if (yaw != 0.0f)
	{
		glm::vec3 axis = GetRight();
		::Rotate(mLook, glm::radians(yaw), axis);
	}
	if (roll != 0.0f)
	{
		glm::vec3 axis = GetLook();
		::Rotate(mLook, glm::radians(roll), axis);
	}
}
GLvoid Object::RotatePosition(const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(degree), glm::normalize(axis));
	mPosition -= pivot;
	MultiplyVector(transform, mPosition);
	mPosition += pivot;
}
GLvoid Object::SetLocalRotation(const glm::vec3& axis, const GLfloat& degree)
{
	mLook = Vector3::Look();

	if (axis == Vector3::Up() || axis == Vector3::Right())
	{
		mLook = Vector3::RotateOrigin(mLook, glm::radians(-degree), axis);
	}
}
GLvoid Object::ResetRotation()
{
	mRotation = { 0, 0, 1.0f, 0 };
}

glm::quat Object::GetRotation() const
{
	return mRotation;
}
GLvoid Object::SetRotationPivot(const glm::vec3* pivot)
{
	mRotationPivot = pivot;
}




GLvoid Object::SetPivot(const glm::vec3* pivot)
{
	mPivot = pivot;
}


GLvoid Object::Look(const glm::vec3& point)
{
	if (mPivot == nullptr)
	{
		mLook = glm::normalize(point - mPosition);
	}
	else
	{
		mLook = glm::normalize(point - (mPosition + *mPivot));
	}
}
GLvoid Object::SetLook(const glm::vec3& look)
{
	mLook = look;
}
GLvoid Object::ResetLook()
{
	mLook = Vector3::Look();
}
glm::vec3 Object::GetLook() const
{
	return mLook;
}
glm::vec3 Object::GetRight() const
{
	glm::vec3 right = glm::cross(mLook, Vector3::Up());

	return right;
}
glm::vec3 Object::GetUp() const
{
	glm::vec3 up = glm::cross(GetRight(), mLook);
	return up;
}


GLvoid Object::SetPosition(const glm::vec3& position)
{
	mPosition = position;
}
GLvoid Object::SetPosX(const GLfloat& x)
{
	mPosition.x = x;
}
GLvoid Object::SetPosY(const GLfloat& y)
{
	mPosition.y = y;
}
GLvoid Object::SetPosZ(const GLfloat& z)
{
	mPosition.z = z;
}
const glm::vec3* Object::GetRefPos() const
{
	return &mPosition;
}
glm::vec3 Object::GetPviotedPosition() const
{
	glm::vec3 result = mPosition;
	if (mPivot) result += *mPivot;
	return result;
}
GLvoid Object::Move(const glm::vec3& vector, const GLboolean& applyTime)
{
	MoveX(vector.x, applyTime);
	MoveY(vector.y, applyTime);
	MoveZ(vector.z, applyTime);
}
GLvoid Object::MoveX(const GLfloat& amount, const GLboolean& applyTime)
{
	glm::vec3 right = GetRight();
	GLfloat dt = 1.0f;
	if (applyTime)
	{
		dt = timer::DeltaTime();
	}

	mPosition += right * amount * dt;
}
GLvoid Object::MoveY(const GLfloat& amount, const GLboolean& applyTime)
{
	glm::vec3 up = GetUp();
	GLfloat dt = 1.0f;
	if (applyTime)
	{
		dt = timer::DeltaTime();
	}

	mPosition += up * amount * dt;
}
GLvoid Object::MoveZ(const GLfloat& amount, const GLboolean& applyTime)
{
	glm::vec3 look = GetLook();
	GLfloat dt = 1.0f;
	if (applyTime)
	{
		dt = timer::DeltaTime();
	}

	mPosition += look * amount * dt;
}
GLvoid Object::MoveGlobal(const glm::vec3& vector, const GLboolean& applyTime)
{
	GLfloat dt = 1.0f;
	if (applyTime)
	{
		dt = timer::DeltaTime();
	}
	mPosition += vector * dt;
}


















ShaderObject::ShaderObject()
{
	InitValues();
	SetColor(DEFAULT_OBJECT_COLOR);
}
ShaderObject::~ShaderObject() {}

GLvoid ShaderObject::PrepareDraw() const
{
	ModelTransform();
	shd::SetShader(mShader, "objectColor", glm::vec3(mColor));
}
GLvoid ShaderObject::InitValues()
{
	Object::InitValues();
	mScale = { 1.0f,1.0f,1.0f };
	mScaleOrigin = { 1.0f,1.0f,1.0f };
}
GLvoid ShaderObject::SetScale(const GLfloat& scale)
{
	mScale.x = scale;
	mScale.y = scale;
	mScale.z = scale;
}
GLvoid ShaderObject::SetScale(const glm::vec3& scale)
{
	mScale = scale;
}
GLvoid ShaderObject::ScaleOrigin(const GLfloat& scale)
{
	Scale(scale);
	mPosition *= scale;
}

GLvoid ShaderObject::Scale(const GLfloat& scale)
{
	mScale *= scale;
}
GLvoid ShaderObject::ScaleX(const GLfloat& amount)
{
	mScale.x *= amount;
}
GLvoid ShaderObject::ScaleY(const GLfloat& amount)
{
	mScale.y *= amount;
}
GLvoid ShaderObject::ScaleZ(const GLfloat& amount)
{
	mScale.z *= amount;
}
GLvoid ShaderObject::SetScaleX(const GLfloat& scale)
{
	mScale.x = scale;
}
GLvoid ShaderObject::SetScaleY(const GLfloat& scale)
{
	mScale.y = scale;
}
GLvoid ShaderObject::SetScaleZ(const GLfloat& scale)
{
	mScale.z = scale;
}


glm::mat4 ShaderObject::GetTransform() const
{
	glm::mat4 transform = glm::mat4(1.0f);

	if (mPivot != nullptr)
	{
		transform = glm::translate(transform, { (*mPivot).x, (*mPivot).y, (*mPivot).z });
	}

	if (mRotationPivot == nullptr)
	{
		transform *= glm::mat4_cast(mRotation);
		transform = glm::translate(transform, { -mPosition.x, mPosition.y, -mPosition.z });	// T
	}
	else
	{
		transform = glm::translate(transform, { -mPosition.x, mPosition.y, -mPosition.z });
		if (mPivot == nullptr)
		{
			transform = glm::translate(transform, -*mRotationPivot);
			transform *= glm::mat4_cast(mRotation);
			transform = glm::translate(transform, *mRotationPivot);
		}
		else
		{
			transform = glm::translate(transform, -*mRotationPivot + (*mPivot));
			transform *= glm::mat4_cast(mRotation);
			transform = glm::translate(transform, *mRotationPivot - (*mPivot));
		}
	}

	glm::quat lookAt = glm::quatLookAt(mLook, Vector3::Up());
	transform *= glm::mat4_cast(lookAt);		// R

	transform = glm::scale(transform, mScale);	// S

	return transform;
}
GLvoid ShaderObject::ModelTransform() const
{
	glm::mat4 transform = GetTransform();
	shd::SetShader(mShader, transform, "modelTransform");
	shd::SetShader(mShader, transform, "normalTransform");
}










IdentityObject::IdentityObject() : ShaderObject()
{
	InitValues();
	InitBuffers();
}
IdentityObject::~IdentityObject()
{
	DeleteBuffers();
}
GLvoid IdentityObject::InitValues()
{
	ShaderObject::InitValues();
	mVAO = 0;
	mVBO[0] = 0;
	mVBO[1] = 0;
	mEBO = 0;

	SetShader(Shader::Light);
}
GLvoid IdentityObject::InitBuffers()
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(2, &mVBO[0]);
	glGenBuffers(1, &mEBO);
}
GLvoid IdentityObject::DeleteBuffers()
{
	glDeleteBuffers(2, &mVBO[0]);
	glDeleteBuffers(1, &mEBO);
	glDeleteVertexArrays(1, &mVAO);
}

GLvoid IdentityObject::BindBuffers()
{
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<size_t> indices;

	PullVertices(vertices);
	PullIndices(indices);

	switch (mShader)
	{
	case Shader::Light:
		PullNormals(normals);
		break;
	}

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	if (normals.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
	}

	if (indices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(size_t) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
	}
	else
	{
		glDeleteBuffers(1, &mEBO);
	}

	glBindVertexArray(0);
}


GLvoid IdentityObject::Draw() const
{
	ShaderObject::PrepareDraw();
	const size_t indexCount = GetIndexCount();

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
GLfloat IdentityObject::GetWidth() const
{
	assert(0);
	return 0;
}
GLfloat IdentityObject::GetHeight() const
{
	assert(0);
	return 0;
}
GLfloat IdentityObject::GetDepth() const
{
	assert(0);
	return 0;
}



SharedObject::SharedObject(const IdentityObject* object) : ShaderObject()
{
	SetObject(object);
	SetShader(object->GetShader());
}

GLvoid SharedObject::Draw() const
{
	ShaderObject::PrepareDraw();
	const size_t indexCount = mObject->GetIndexCount();

	glBindVertexArray(mObject->GetVAO());
	if (indexCount > 0)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		const size_t vertexCount = mObject->GetVertexCount();
		switch (vertexCount)
		{
		case 1:
			glDrawArrays(GL_POINT, 0, 1);
			break;
		case 2:
			glDrawArrays(GL_LINES, 0, 2);
			break;
		default:
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
			break;
		}
	}
	glBindVertexArray(0);
}











///// private /////
ModelObject::ModelObject() : IdentityObject()
{

}
ModelObject::ModelObject(const Model* model) : IdentityObject()
{
	LoadModel(model);
}
GLvoid ModelObject::LoadModel(const Model* model)
{
	if (mModel != nullptr)
	{
		DeleteBuffers();
	}

	IdentityObject::InitValues();
	InitBuffers();
	mModel = model;

	SetShader(Shader::Light);
	BindBuffers();
}
GLvoid ModelObject::PullNormals(vector<GLfloat>& normals) const
{
	const vector<glm::vec3> objectNormals = mModel->GetNormals();
	const vector<size_t> normalIndices = mModel->GetNormalIndices();
	const vector<size_t> vertexIndices = mModel->GetIndices();

	// vertex마다 normal을 지정
	normals.resize(mModel->GetVertexCount() * 3, 0);
	size_t count = 0;
	for (const size_t& vertexIndex : vertexIndices)
	{
		glm::vec3 normal = objectNormals[normalIndices[count]];
		normals[(vertexIndex * 3)] = normal.x;
		normals[(vertexIndex * 3) + 1] = normal.y;
		normals[(vertexIndex * 3) + 2] = normal.z;
		++count;
	}
}
GLvoid ModelObject::PullVertices(vector<GLfloat>& vertices) const
{
	const vector<glm::vec3> objectVertices = mModel->GetVertices();
	for (size_t i = 0; i < objectVertices.size(); ++i)
	{
		vertices.emplace_back(objectVertices[i].x);
		vertices.emplace_back(objectVertices[i].y);
		vertices.emplace_back(objectVertices[i].z);
	}
}
GLvoid ModelObject::PullIndices(vector<size_t>& vertexIndices) const
{
	const vector<size_t> objectIndices = mModel->GetIndices();
	for (const size_t& index : objectIndices)
	{
		vertexIndices.emplace_back(index);
	}
}
GLvoid ModelObject::PullNormalIndices(vector<size_t>& normalIndices) const
{
	const vector<size_t> indices = mModel->GetNormalIndices();
	for (const size_t& index : indices)
	{
		normalIndices.emplace_back(index);
	}
}


///// public /////
size_t ModelObject::GetIndexCount() const
{
	return mModel->GetIndexCount();
}
size_t ModelObject::GetVertexCount() const
{
	return mModel->GetVertexCount();
}
GLfloat ModelObject::GetSide(const Dir& dir) const
{
	GLfloat width = ModelObject::GetWidth();
	GLfloat height = ModelObject::GetHeight();
	GLfloat depth = ModelObject::GetDepth();

	switch (dir)
	{
	case Dir::Left:
	{
		GLfloat left = mPosition.x - width / 2;
		if (mPivot)
		{
			left += mPivot->x;
		}
		return left;
	}
	case Dir::Right:
	{
		GLfloat right = mPosition.x + width / 2;
		if (mPivot)
		{
			right -= mPivot->x;
		}
		return right;
	}
	case Dir::Down:
	{
		GLfloat bottom = mPosition.y - height / 2;
		if (mPivot)
		{
			bottom += mPivot->y;
		}
		return bottom;
	}
	case Dir::Up:
	{
		GLfloat top = mPosition.y + height / 2;
		if (mPivot)
		{
			top -= mPivot->y;
		}
		return top;
	}
	case Dir::Front:
	{
		GLfloat front = mPosition.z - depth / 2;
		if (mPivot)
		{
			front -= mPivot->z;
		}
		return front;
	}
	case Dir::Back:
	{
		GLfloat back = mPosition.z + depth / 2;
		if (mPivot)
		{
			back += mPivot->z;
		}
		return back;
	}
	default:
		assert(0);
		return -1;
	}
}
GLfloat ModelObject::GetWidth() const
{
	GLfloat width = mModel->GetWidth();
	return width * mScale.x;
}
GLfloat ModelObject::GetHeight() const
{
	GLfloat height = mModel->GetHeight();
	return height * mScale.y;
}
GLfloat ModelObject::GetDepth() const
{
	GLfloat depth = mModel->GetDepth();
	return depth * mScale.z;
}
GLrect ModelObject::GetXZRect() const
{
	Cuboid cuboid(&mPosition, &mScale, GetWidth(), GetHeight(), GetDepth());
	return cuboid.GetXZRect();
}



///// private /////
GLvoid CustomObject::PullColors(vector<GLfloat>& colors) const
{
	for (size_t i = 0; i < mVertices.size(); ++i)
	{
		colors.emplace_back(mColor.r);
		colors.emplace_back(mColor.g);
		colors.emplace_back(mColor.b);
	}
}
GLvoid CustomObject::PullVertices(vector<GLfloat>& vertices) const
{
	for (size_t i = 0; i < mVertices.size(); ++i)
	{
		vertices.emplace_back(mVertices[i].x);
		vertices.emplace_back(mVertices[i].y);
		vertices.emplace_back(mVertices[i].z);
	}
}
GLvoid CustomObject::PullIndices(vector<size_t>& vertexIndices) const
{
	for (const size_t& index : mIndices)
	{
		vertexIndices.emplace_back(index);
	}
}
size_t CustomObject::GetIndexCount() const
{
	return mIndices.size();
}
size_t CustomObject::GetVertexCount() const
{
	return mVertices.size();
}


///// public /////
CustomObject::CustomObject() : IdentityObject()
{
	SetShader(Shader::Color);
}
CustomObject::CustomObject(vector<glm::vec3>& vertices) : CustomObject()
{
	mVertices = vertices;
	BindBuffers();
}
CustomObject::CustomObject(vector<glm::vec3>& vertices, vector<size_t>& indices) : CustomObject()
{
	mVertices = vertices;
	mIndices = indices;
	BindBuffers();
}
set<glm::vec2, CompareSet> ModelObject::GetBoundings_XZ() const
{
	return mModel->GetBoundings_XZ();
}












////////// [ BASIC OBJECTS ] //////////

// Line
Line::Line() : CustomObject()
{
	glm::vec3 v1 = { 0, 0, 1.0f };
	glm::vec3 v2 = { 0, 0, -1.0f };
	mVertices.emplace_back(v1);
	mVertices.emplace_back(v2);
	BindBuffers();
}
Line::Line(const glm::vec3& v1, const glm::vec3& v2) : CustomObject()
{
	mVertices.emplace_back(v1);
	mVertices.emplace_back(v2);
	BindBuffers();
}
GLvoid Line::Draw() const
{
	ShaderObject::PrepareDraw();

	glBindVertexArray(mVAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}
GLvoid Line::SetVertex(const GLboolean& index, const glm::vec3& pos)
{
	mVertices[index] = pos;
}

// Triangle
Triangle::Triangle() : CustomObject()
{
	const vector<glm::vec3> defaultTriangle = {
		{  0, 0, -1},
		{ -1, 0,  1},
		{  1, 0,  1},
	};
	mVertices = defaultTriangle;

	BindBuffers();
}
GLvoid Triangle::Draw() const
{
	ShaderObject::PrepareDraw();

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

// Plane
Plane::Plane() : CustomObject()
{
	const vector<glm::vec3> defaultPlane = {
		{-1, 0, -1},
		{-1, 0,  1},
		{ 1, 0, -1},
		{ 1, 0,  1},
	};
	mVertices = defaultPlane;

	BindBuffers();
}
GLvoid Plane::Draw() const
{
	ShaderObject::PrepareDraw();

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
glm::vec3 Plane::GetNormal() const
{
	glm::vec3 vertices[3] = { mVertices[0],mVertices[1] ,mVertices[2] };

	glm::mat4 transform = GetTransform();
	for (glm::vec3& vertex : vertices)
	{
		MultiplyVector(transform, vertex);
	}

	glm::vec3 v1 = vertices[0];
	glm::vec3 v2 = vertices[1];
	glm::vec3 v3 = vertices[2];

	return glm::normalize(glm::cross(v3 - v2, v1 - v2));
}
GLfloat Plane::CheckCollision(const glm::vec3& center, const GLfloat& radius) const
{
	glm::vec3 vertices[3] = { mVertices[0],mVertices[1] ,mVertices[2] };

	glm::mat4 transform = GetTransform();
	for (glm::vec3& vertex : vertices)
	{
		MultiplyVector(transform, vertex);
	}

	glm::vec3 v1 = vertices[0];
	glm::vec3 v2 = vertices[1];
	glm::vec3 v3 = vertices[2];
	glm::vec3 normal = glm::normalize(glm::cross(v3 - v2, v1 - v2));

	GLfloat distance = fabs(glm::dot((center - v1), normal) / glm::length(normal));

	GLfloat interRadius = sqrtf((radius * radius) - (distance * distance));
	glm::vec3 point = center + distance * normal;
	if ((distance < radius) && (interRadius > 0))
	{
		return radius - distance;
	}

	return 0.0f;
}


// Cuboid
Cuboid::Cuboid(const IdentityObject* object)
{
	mPosition = object->GetRefPos();
	mScale = object->GetRefScale();
	mHalfWidth = object->GetWidth() / 2;
	mHeight = object->GetHeight();
	mHalfDepth = object->GetDepth() / 2;
}
Cuboid::Cuboid(const SharedObject* object, const GLfloat& width, const GLfloat& height, const GLfloat& depth)
{
	mPosition = object->GetRefPos();
	mScale = object->GetRefScale();
	mHalfWidth = width;
	mHeight = height;
	mHalfDepth = depth;
}
Cuboid::Cuboid(const glm::vec3* position, const glm::vec3* scale, const GLfloat& width, const GLfloat& height, const GLfloat& depth)
{
	mPosition = position;
	mScale = scale;
	mHalfWidth = width / 2;
	mHeight = height;
	mHalfDepth = depth / 2;
}

GLfloat Cuboid::GetSide(const Dir& dir) const
{
	GLfloat w = mHalfWidth * mScale->x;
	GLfloat h = mHeight * mScale->y;
	GLfloat d = mHalfDepth * mScale->z;

	if (mScale->x == 0)
	{
		w = 0;
	}
	if (mScale->y == 0)
	{
		h = 0;
	}
	if (mScale->z == 0)
	{
		d = 0;
	}

	switch (dir)
	{
	case Dir::Left:
		return mPosition->x - w;
	case Dir::Right:
		return mPosition->x + w;
	case Dir::Down:
		return mPosition->y;
	case Dir::Up:
		return mPosition->y + h;
	case Dir::Front:
		return mPosition->z - d;
	case Dir::Back:
		return mPosition->z + d;
	default:
		assert(0);
		return 0;
	}
}


GLrect Cuboid::GetXZRect() const
{
	GLrect rect;
	rect.left = GetSide(Dir::Left);
	rect.right = GetSide(Dir::Right);
	rect.top = GetSide(Dir::Back);
	rect.bottom = GetSide(Dir::Front);

	return rect;
}
GLboolean Cuboid::CheckCollide(const glm::vec3& point, const GLfloat& radius) const
{
	const GLfloat w = mHalfWidth / mScale->x;
	const GLfloat h = mHeight / mScale->y;
	const GLfloat d = mHalfDepth / mScale->z;

	const GLfloat left = mPosition->x - w + radius;
	const GLfloat right = mPosition->x + w - radius;
	const GLfloat top = mPosition->y + h - radius;
	const GLfloat bottom = mPosition->y - h + radius;
	const GLfloat front = mPosition->z - d + radius;
	const GLfloat back = mPosition->z + d - radius;

	if( (point.x < right && point.x > left) &&
		(point.y < top && point.y > bottom) &&
		(point.z < back && point.z > front))
	{
		return true;
	}

	return false;
}
GLboolean Cuboid::CheckCollide(const GLrect& rect) const
{
	const GLfloat w = mHalfWidth / mScale->x;
	const GLfloat h = mHeight / mScale->y;
	const GLfloat d = mHalfDepth / mScale->z;

	const GLfloat left = mPosition->x - w;
	const GLfloat right = mPosition->x + w;
	const GLfloat front = mPosition->z - d;
	const GLfloat back = mPosition->z + d;

	if (((rect.left > left && rect.left < right) || (rect.right > left && rect.right < right)) &&
		((rect.top > front && rect.top < back) || (rect.bottom > front && rect.bottom < back)))
	{
		return true;
	}

	return false;
}
GLvoid Cuboid::Draw() const
{
	const GLfloat w = mHalfWidth * mScale->x;
	const GLfloat h = (mHeight/2) * mScale->y;
	const GLfloat d = mHalfDepth * mScale->z;

	const Line* identityLine = GetIdentityLine();
	SharedObject line(identityLine);

	// z
	line.SetPosition(*mPosition);
	line.SetScaleZ(d);
	line.SetColor(BLUE);
	for (size_t i = 0; i < 2; ++i)
	{
		if (i == 0)
		{
			line.SetPosY(GetSide(Dir::Down));
		}
		else
		{
			line.SetPosY(GetSide(Dir::Up));
		}

		line.SetPosX(GetSide(Dir::Left));
		line.Draw();
		line.SetPosX(GetSide(Dir::Right));
		line.Draw();
	}

	// x
	line.SetPosition(*mPosition);
	line.SetScaleZ(w);
	line.SetColor(RED);
	line.RotateLocal(0, 90, 0);
	for (size_t i = 0; i < 2; ++i)
	{
		if (i == 0)
		{
			line.SetPosY(GetSide(Dir::Down));
		}
		else
		{
			line.SetPosY(GetSide(Dir::Up));
		}

		line.SetPosZ(GetSide(Dir::Front));
		line.Draw();
		line.SetPosZ(GetSide(Dir::Back));
		line.Draw();
	}

	// y
	line.SetPosition(*mPosition);
	line.SetScaleZ(h);
	line.MoveY(h);
	line.SetColor(GREEN);
	line.RotateLocal(Vector3::Right(), -90);
	for (size_t i = 0; i < 2; ++i)
	{
		if (i == 0)
		{
			line.SetPosX(GetSide(Dir::Left));
		}
		else
		{
			line.SetPosX(GetSide(Dir::Right));
		}

		line.SetPosZ(GetSide(Dir::Front));
		line.Draw();
		line.SetPosZ(GetSide(Dir::Back));
		line.Draw();
	}
}


Circle::Circle(const glm::vec3* position, const GLfloat& radius, const glm::vec3 offset)
{
	mOffset = offset;
	mRadius = radius;
	mCircle = new SharedObject(GetIdentityModelObject(IdentityObjects::Circle));
	mCircle->SetScale(radius);
	mCircle->SetPivot(position);
	mCircle->Move(offset);
}
GLvoid Circle::Draw() const
{
	mCircle->Draw();
}
GLvoid Circle::SetColor(const COLORREF& color)
{
	mCircle->SetColor(color);
}
glm::vec2 Circle::GetCenter() const
{
	glm::vec3 pos = mCircle->GetPviotedPosition();
	return { pos.x, pos.z };
}
GLfloat Circle::GetRadius() const
{
	return mRadius;
}



Cube::Cube() : ModelObject()
{
	const Model* model = GetModel(Models::Cube);
	ModelObject::LoadModel(model);
}
GLvoid Cube::SetChild(Cube* cube)
{
	cube->SetPivot(&mPosition);
	if (mPivot)
	{
		cube->SetPivot(mPivot);
	}
	mChilds.emplace_back(cube);
}
GLvoid Cube::AddPivot(const glm::vec3& pivot)
{
	mPivots.emplace_back(pivot);
}
const glm::vec3* Cube::GetPivot(const size_t& index)
{
	::CheckOutOfIndex(index, mPivots.size());

	return &mPivots[index];
}

GLvoid Cube::Draw() const
{
	IdentityObject::Draw();
	for (Cube* child : mChilds)
	{
		child->Draw();
	}
}
GLvoid Cube::Move(const glm::vec3& vector)
{
	IdentityObject::Move(vector);
}
GLvoid Cube::Rotate(const glm::vec3& pivot, const GLfloat& degree)
{
	IdentityObject::Rotate(pivot, degree);
	for (Cube* child : mChilds)
	{
		child->Rotate(pivot, degree);
	}
}
GLvoid Cube::RotateLocal(const glm::vec3& pivot, const GLfloat& degree)
{
	IdentityObject::RotateLocal(pivot, degree);
	for (Cube* child : mChilds)
	{
		child->Rotate(pivot, degree);
	}
}

GLboolean Cube::CheckCollide(const glm::vec3& point) const
{
	Cuboid cuboid(this);
	return cuboid.CheckCollide(point);
}
GLboolean Cube::CheckCollide(const GLrect& rect) const
{
	Cuboid cuboid(this);
	return cuboid.CheckCollide(rect);
}




Sphere::Sphere()
{
	const Model* model = GetModel(Models::GeoSphere);
	ModelObject::LoadModel(model);
}







GLvoid ICollisionable_2D::SetCollision(const glm::vec3* position, const GLfloat& radius)
{
	mPosition = position;
	mRadius = radius;
	mCollisionType = CollisionType::Circle;
}
GLvoid ICollisionable_2D::SetCollision(const glm::vec3* position, const GLfloat& width, const GLfloat& depth)
{
	mPosition = position;
	mWidth = width;
	mDepth = depth;
	mCollisionType = CollisionType::Rect;
}

GLfloat ICollisionable_2D::GetRadius() const
{
	assert(mCollisionType == CollisionType::Circle);
	return mRadius;
}
GLfloat ICollisionable_2D::GetWidth() const
{
	assert(mCollisionType == CollisionType::Rect);
	return mWidth;
}
GLfloat ICollisionable_2D::GetDepth() const
{
	assert(mCollisionType == CollisionType::Rect);
	return mDepth;
}


CollisionManager::CollisionManager()
{
	mMonsterList.reserve(100);
	mObstacleList.reserve(100);
}
CollisionManager::~CollisionManager()
{

}

GLvoid CollisionManager::CheckCollisions()
{
	for (ICollisionable_2D* monster : mMonsterList)
	{
		//monster
	}
}
GLvoid CollisionManager::AddObject(const Tag& tag, ICollisionable_2D* object)
{
}
GLvoid CollisionManager::DeleteObject(ICollisionable_2D* object)
{
}
























// Bug : does not work correctly when a vertices size is large
GLvoid DrawDebugWireXZ(const set<glm::vec2, CompareSet>& vertices, GLfloat yPos, const COLORREF& color, const glm::vec3* pivot)
{
	if (pivot != nullptr)
	{
		yPos += pivot->y;
	}

	Line* line = nullptr;

	for (glm::vec2 v1 : vertices)
	{
		if (pivot != nullptr)
		{
			v1.x += pivot->x;
			v1.y += pivot->z;
		}
		for (glm::vec2 v2 : vertices)
		{
			if (v1 == v2)
			{
				continue;
			}
			if (pivot != nullptr)
			{
				v2.x += pivot->x;
				v2.y += pivot->z;
			}

			line = new Line({ v1.x , yPos, v1.y }, { v2.x , yPos, v2.y });
			line->SetColor(color);
			line->BindBuffers();
			line->Draw();
			delete line;
		}
	}
}









// identity simple objects
static const Line* lineObject = nullptr;
static const Cube* cubeObject = nullptr;
static const Sphere* sphereObject = nullptr;

// identity model objects
static const ModelObject* circleObject = nullptr;
static const ModelObject* lowSphereObject = nullptr;
static const ModelObject* playerObject = nullptr;
static const ModelObject* gunObject = nullptr;
static const ModelObject* blooperObject = nullptr;

unordered_map<IdentityObjects, pair<const Models, const ModelObject*>> objectMap{
	{IdentityObjects::Circle, make_pair(Models::Circle, circleObject)},
	{IdentityObjects::LowSphere, make_pair(Models::LowSphere, lowSphereObject)},
	{IdentityObjects::Player, make_pair(Models::Player, playerObject)},
	{IdentityObjects::Gun, make_pair(Models::Gun, gunObject)},
	{IdentityObjects::Blooper, make_pair(Models::Blooper, blooperObject)},
};

GLvoid InitObjects()
{
	lineObject = new Line();
	sphereObject = new Sphere();
	cubeObject = new Cube();

	for (auto& iter : objectMap)
	{
		const Model* model = GetModel(iter.second.first);
		iter.second.second = new ModelObject(model);
	}
}
const Line* GetIdentityLine()
{
	return lineObject;
}

const Sphere* GetIdentitySphere()
{
	return sphereObject;
}

const Cube* GetIdentityCube()
{
	return cubeObject;
}
const ModelObject* GetIdentityModelObject(const IdentityObjects& object)
{
	return objectMap[object].second;
}






static vector<ShaderObject*> customObjects;
static vector<ShaderObject*> lightObjects;
static vector<ShaderObject*> minimapObjects;


GLvoid AddObject(const Shader& shader, ShaderObject* object)
{
	switch (shader)
	{
	case Shader::Color:
		customObjects.emplace_back(object);
		break;
	case Shader::Light:
		lightObjects.emplace_back(object);
		break;
	default:
		assert(0);
		break;
	}
}
GLvoid DeleteObject(const Shader& shader, ShaderObject* object)
{

}
GLvoid ResetObjects()
{
	for (ShaderObject* object : customObjects)
	{
		delete object;
	}
	for (ShaderObject* object : lightObjects)
	{
		delete object;
	}
	for (ShaderObject* object : minimapObjects)
	{
		delete object;
	}
	customObjects.clear();
	lightObjects.clear();
	minimapObjects.clear();
}


GLvoid DrawObjects(const Shader& shader)
{
	switch (shader)
	{
	case Shader::Color:
		for (const ShaderObject* object : customObjects)
		{
			object->Draw();
		}
		break;
	case Shader::Light:
		for (const ShaderObject* object : lightObjects)
		{
			object->Draw();
		}
		break;
	default:
		break;
	}

}