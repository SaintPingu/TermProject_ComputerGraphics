#include "stdafx.h"
#include "Model.h"
#include "Object.h"
#include "Transform.h"
#include "Timer.h"

extern vector<IdentityObject*> objects;
extern const Model* cubeModel;
extern const Model* sphereModel;
extern const Model* playerModel;


Object::Object()
{
	InitValues();
}
Object::~Object() {}

GLvoid Object::InitValues()
{
	position = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f,1.0f,1.0f };
	scaleOrigin = { 1.0f,1.0f,1.0f };

	look = { 0.0f, 0.0f, 1.0f };
	rotation = { 0.0f, 0.0f, 1.0f, 0.0f };
	localRotation = { 0.0f, 0.0f, 1.0f, 0.0f };

	pivot = nullptr;
}
GLvoid Object::SetScale(const GLfloat& scale)
{
	this->scale.x = scale;
	this->scale.y = scale;
	this->scale.z = scale;
}
GLvoid Object::SetScale(const glm::vec3& scale)
{
	this->scale = scale;
}
GLvoid Object::ScaleOrigin(const GLfloat& scale)
{
	Scale(scale);
	position *= scale;
}

GLvoid Object::Scale(const GLfloat& scale)
{
	this->scale *= scale;
}
GLvoid Object::ScaleX(const GLfloat& amount)
{
	scale.x *= amount;
}
GLvoid Object::ScaleY(const GLfloat& amount)
{
	scale.y *= amount;
}
GLvoid Object::ScaleZ(const GLfloat& amount)
{
	scale.z *= amount;
}
GLvoid Object::SetScaleX(const GLfloat& scale)
{
	this->scale.x = scale;
}
GLvoid Object::SetScaleY(const GLfloat& scale)
{
	this->scale.y = scale;
}
GLvoid Object::SetScaleZ(const GLfloat& scale)
{
	this->scale.z = scale;
}
const glm::vec3* Object::GetRefScale() const
{
	return &scale;
}


GLvoid Object::Rotate(const glm::vec3& axis, const GLfloat& degree)
{
	rotation *= glm::angleAxis(glm::radians(degree), glm::normalize(axis));
}
GLvoid Object::RotatePivot(const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree)
{
	RotateLocal(axis, degree);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(-degree), glm::normalize(axis));
	position -= pivot;
	MultiplyVector(transform, position);
	position += pivot;
}
GLvoid Object::RotateLocal(const glm::vec3& axis, const GLfloat& degree)
{
	localRotation *= glm::angleAxis(glm::radians(degree), glm::normalize(axis));

	::Rotate(look, degree, axis);
}
GLvoid Object::RotateLocal(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll)
{
	if (yaw != 0.0f)
	{
		glm::vec3 axis = GetRight();
		localRotation *= glm::angleAxis(glm::radians(yaw), axis);
		::Rotate(look, glm::radians(yaw), axis);
	}
	if (pitch != 0.0f)
	{
		glm::vec3 axis = GetUp();
		localRotation *= glm::angleAxis(glm::radians(pitch), axis);
		::Rotate(look, glm::radians(pitch), axis);
	}
	if (roll != 0.0f)
	{
		glm::vec3 axis = GetLook();
		localRotation *= glm::angleAxis(glm::radians(roll), axis);
		::Rotate(look, glm::radians(roll), axis);
	}
}
GLvoid Object::RotatePosition(const glm::vec3& pivot, const glm::vec3& axis, const GLfloat& degree)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(degree), glm::normalize(axis));
	position -= pivot;
	MultiplyVector(transform, position);
	position += pivot;
}
GLvoid Object::SetLocalRotation(const glm::vec3& axis, const GLfloat& degree)
{
	localRotation = { 0.0f, 0.0f, 1.0f, 0.0f };
	look = { 0.0f, 0.0f, 1.0f };
	localRotation *= glm::angleAxis(glm::radians(degree), glm::normalize(axis));

	if (axis == Vector3::Up() || axis == Vector3::Right())
	{
		look = Vector3::RotateOrigin(look, glm::radians(-degree), axis);
	}
}
GLvoid Object::ResetRotation()
{
	rotation = { 0, 0, 1.0f, 0 };
}
GLvoid Object::ResetLocalRotation()
{
	localRotation = { 0, 0, 1.0f, 0 };
}

glm::quat Object::GetRotation() const
{
	return rotation;
}
glm::quat Object::GetLocalRotation() const
{
	return localRotation;
}
GLvoid Object::SetRotationPivot(const glm::vec3* pivot)
{
	this->rotationPivot = pivot;
}


GLvoid Object::SetPivot(const glm::vec3* pivot)
{
	this->pivot = pivot;
}

GLvoid Object::SetLook(const glm::vec3& look)
{
	this->look = look;
}
glm::vec3 Object::GetLook() const
{
	return look;
}
glm::vec3 Object::GetRight() const
{
	glm::vec3 right = glm::cross(look, Vector3::Up());

	return right;
}
glm::vec3 Object::GetUp() const
{
	glm::vec3 up = glm::cross(GetRight(), look);
	return up;
}


GLvoid Object::SetPosition(const glm::vec3& position)
{
	this->position = position;
}
GLvoid Object::SetPosX(const GLfloat& amount)
{
	position.x = amount;
}
GLvoid Object::SetPosY(const GLfloat& amount)
{
	position.y = amount;
}
GLvoid Object::SetPosZ(const GLfloat& amount)
{
	position.z = amount;
}
const glm::vec3* Object::GetRefPos() const
{
	return &position;
}
glm::vec3 Object::GetPosition() const
{
	glm::vec3 result = position;
	if (pivot) result += *pivot;
	return result;
}
GLvoid Object::Move(const glm::vec3& vector)
{
	MoveX(vector.x);
	MoveY(vector.y);
	MoveZ(vector.z);
}
GLvoid Object::MoveX(const GLfloat& amount)
{
	glm::vec3 right = GetRight();
	this->position += right * amount * timer::DeltaTime();
}
GLvoid Object::MoveY(const GLfloat& amount)
{
	glm::vec3 up = GetUp();
	this->position += up * amount * timer::DeltaTime();
}
GLvoid Object::MoveZ(const GLfloat& amount)
{
	glm::vec3 look = GetLook();
	this->position += look * amount * timer::DeltaTime();
}
GLvoid Object::MoveGlobal(const glm::vec3& vector)
{
	this->position += vector * timer::DeltaTime();
}


glm::mat4 Object::GetTransform() const
{
	glm::mat4 transform = glm::mat4(1.0f);

	if (pivot != nullptr)
	{
		transform = glm::translate(transform, { -(*pivot).x, (*pivot).y, -(*pivot).z });
	}

	transform *= glm::mat4_cast(rotation);
	transform = glm::translate(transform, { -position.x, position.y, -position.z });

	if (rotationPivot == nullptr)
	{
		transform *= glm::mat4_cast(localRotation);
	}
	else
	{
		transform = glm::translate(transform, -*rotationPivot);
		transform *= glm::mat4_cast(localRotation);
		transform = glm::translate(transform, *rotationPivot);
	}

	transform = glm::scale(transform, scale);

	return transform;
}
GLvoid Object::ModelTransform() const
{
	glm::mat4 transform = transform::GetWorld();
	transform *= GetTransform();
	transform::Apply(transform, "modelTransform");
}











IdentityObject::~IdentityObject()
{
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(2, &VBO[0]);
	glDeleteBuffers(1, &EBO);
}
GLvoid IdentityObject::InitValues()
{
	VAO = 0;
	VBO[0] = 0;
	VBO[1] = 0;
	EBO = 0;
}
GLvoid IdentityObject::InitBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, &VBO[0]);
	glGenBuffers(1, &EBO);
}


GLvoid IdentityObject::BindBuffers()
{
	vector<GLfloat> vertices;
	vector<GLfloat> colors;
	vector<size_t> indices;

	PullVertices(vertices);
	PullColors(colors);
	PullIndices(indices);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), colors.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	if (indices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(size_t) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
	}
	else
	{
		glDeleteBuffers(1, &EBO);
	}

	glBindVertexArray(0);
}

GLvoid IdentityObject::Draw() const
{
	const size_t indexCount = GetIndexCount();
	IdentityObject::ModelTransform();

	glBindVertexArray(VAO);
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



SharedObject::SharedObject(const IdentityObject* object)
{
	SetObject(object);
}

GLvoid SharedObject::Draw() const
{
	const size_t indexCount = this->object->GetIndexCount();
	SharedObject::ModelTransform();

	if (isChangeColor)
	{
		transform::ApplyColor(color);
	}

	glBindVertexArray(this->object->GetVAO());
	if (indexCount > 0)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		const size_t vertexCount = this->object->GetVertexCount();
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

	if (isChangeColor)
	{
		transform::DisableColor();
	}
}

GLvoid SharedObject::SetColor(const COLORREF& color)
{
	this->color = MyColor(color);
	isChangeColor = true;
}
GLvoid SharedObject::DisableColor()
{
	isChangeColor = false;
}














///// private /////
ModelObject::ModelObject(const Model* model)
{
	this->LoadModel(model);
}
GLvoid ModelObject::LoadModel(const Model* model)
{
	IdentityObject::InitValues();
	InitBuffers();
	this->model = model;
}
GLvoid ModelObject::PullColors(vector<GLfloat>& colors) const
{
	for (size_t i = 0; i < this->colors.size(); ++i)
	{
		colors.emplace_back(this->colors[i].r);
		colors.emplace_back(this->colors[i].g);
		colors.emplace_back(this->colors[i].b);
	}
}
GLvoid ModelObject::PullVertices(vector<GLfloat>& vertices) const
{
	const vector<glm::vec3> objectVertices = model->GetVertices();
	for (size_t i = 0; i < objectVertices.size(); ++i)
	{
		vertices.emplace_back(objectVertices[i].x);
		vertices.emplace_back(objectVertices[i].y);
		vertices.emplace_back(objectVertices[i].z);
	}
}
GLvoid ModelObject::PullIndices(vector<size_t>& vertexIndices) const
{
	const vector<size_t> objectIndices = model->GetIndices();
	for (const size_t& index : objectIndices)
	{
		vertexIndices.emplace_back(index);
	}
}



///// public /////
size_t ModelObject::GetIndexCount() const
{
	return model->GetIndexCount();
}
size_t ModelObject::GetVertexCount() const
{
	return model->GetVertexCount();
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
		GLfloat left = position.x - width / 2;
		if (pivot)
		{
			left += pivot->x;
		}
		return left;
	}
	case Dir::Right:
	{
		GLfloat right = position.x + width / 2;
		if (pivot)
		{
			right -= pivot->x;
		}
		return right;
	}
	case Dir::Down:
	{
		GLfloat bottom = position.y - height / 2;
		if (pivot)
		{
			bottom += pivot->y;
		}
		return bottom;
	}
	case Dir::Up:
	{
		GLfloat top = position.y + height / 2;
		if (pivot)
		{
			top -= pivot->y;
		}
		return top;
	}
	case Dir::Front:
	{
		GLfloat front = position.z - depth / 2;
		if (pivot)
		{
			front -= pivot->z;
		}
		return front;
	}
	case Dir::Back:
	{
		GLfloat back = position.z + depth / 2;
		if (pivot)
		{
			back += pivot->z;
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
	GLfloat width = model->GetWidth();
	return width * scale.x;
}
GLfloat ModelObject::GetHeight() const
{
	GLfloat height = model->GetHeight();
	return height * scale.y;
}
GLfloat ModelObject::GetDepth() const
{
	GLfloat depth = model->GetDepth();
	return depth * scale.z;
}

GLvoid ModelObject::ClearColor()
{
	colors.clear();
	colors.reserve(model->GetVertexCount());
}
GLvoid ModelObject::SetColor(const COLORREF& color)
{
	ClearColor();

	MyColor myColor;
	myColor.SetColor(color);
	for (size_t i = 0; i < model->GetVertexCount(); ++i)
	{
		colors.emplace_back(myColor);
	}
}
GLvoid ModelObject::SetColor(const size_t& index, const MyColor& color)
{
	::CheckOutOfIndex(index, colors.size());

	colors[index] = color;
}
GLvoid ModelObject::RandomizeColor()
{
	ClearColor();
	for (size_t i = 0; i < model->GetVertexCount(); ++i)
	{
		MyColor color;
		color.Randomize();
		colors.emplace_back(color);
	}
}
GLrect ModelObject::GetXZRect() const
{
	Cuboid cuboid(&position, &scale, GetWidth(), GetHeight(), GetDepth());
	return cuboid.GetXZRect();
}




///// private /////
GLvoid CustomObject::PullColors(vector<GLfloat>& colors) const
{
	for (size_t i = 0; i < this->colors.size(); ++i)
	{
		colors.emplace_back(this->colors[i].r);
		colors.emplace_back(this->colors[i].g);
		colors.emplace_back(this->colors[i].b);
	}
}
GLvoid CustomObject::PullVertices(vector<GLfloat>& vertices) const
{
	for (size_t i = 0; i < this->vertices.size(); ++i)
	{
		vertices.emplace_back(this->vertices[i].x);
		vertices.emplace_back(this->vertices[i].y);
		vertices.emplace_back(this->vertices[i].z);
	}
}
GLvoid CustomObject::PullIndices(vector<size_t>& vertexIndices) const
{
	for (const size_t& index : indices)
	{
		vertexIndices.emplace_back(index);
	}
}
size_t CustomObject::GetIndexCount() const
{
	return indices.size();
}
size_t CustomObject::GetVertexCount() const
{
	return vertices.size();
}


///// public /////
CustomObject::CustomObject()
{
	IdentityObject::InitValues();
	InitBuffers();
}
CustomObject::CustomObject(vector<glm::vec3>& vertices) : CustomObject()
{
	this->vertices = vertices;
	colors.resize(vertices.size());
}
CustomObject::CustomObject(vector<glm::vec3>& vertices, vector<size_t>& indices) : CustomObject(vertices)
{
	this->indices = indices;
}
GLvoid CustomObject::SetColor(const COLORREF& color)
{
	colors.clear();
	colors.resize(vertices.size());
	for (size_t i = 0; i < colors.size(); ++i)
	{
		colors[i].SetColor(color);
	}
}
GLvoid CustomObject::SetColor(const size_t& index, const COLORREF& color)
{
	::CheckOutOfIndex(index, colors.size());

	colors[index].SetColor(color);
}














////////// [ BASIC OBJECTS ] //////////

// Line
Line::Line()
{
	glm::vec3 v1 = { 0, 0, 1.0f };
	glm::vec3 v2 = { 0, 0, -1.0f };
	vertices.emplace_back(v1);
	vertices.emplace_back(v2);
}
Line::Line(const glm::vec3& v1, const glm::vec3& v2) : CustomObject()
{
	vertices.emplace_back(v1);
	vertices.emplace_back(v2);
}
GLvoid Line::Draw() const
{
	IdentityObject::ModelTransform();

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

// Triangle
Triangle::Triangle()
{
	const vector<glm::vec3> defaultTriangle = {
		{  0, 0, -1},
		{ -1, 0,  1},
		{  1, 0,  1},
	};
	vertices = defaultTriangle;
}
GLvoid Triangle::Draw() const
{
	IdentityObject::ModelTransform();

	glBindVertexArray(VAO);
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
	vertices = defaultPlane;
}
GLvoid Plane::Draw() const
{
	IdentityObject::ModelTransform();

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
glm::vec3 Plane::GetNormal() const
{
	glm::vec3 vertices[3] = { this->vertices[0],this->vertices[1] ,this->vertices[2] };

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
	glm::vec3 vertices[3] = { this->vertices[0],this->vertices[1] ,this->vertices[2] };

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
	position = object->GetRefPos();
	scale = object->GetRefScale();
	halfWidth = object->GetWidth() / 2;
	height = object->GetHeight();
	halfDepth = object->GetDepth() / 2;
}
Cuboid::Cuboid(const SharedObject* object, const GLfloat& width, const GLfloat& height, const GLfloat& depth)
{
	position = object->GetRefPos();
	scale = object->GetRefScale();
	this->halfWidth = width;
	this->height = height;
	this->halfDepth = depth;
}
Cuboid::Cuboid(const glm::vec3* position, const glm::vec3* scale, const GLfloat& width, const GLfloat& height, const GLfloat& depth)
{
	this->position = position;
	this->scale = scale;
	this->halfWidth = width / 2;
	this->height = height;
	this->halfDepth = depth / 2;
}

GLfloat Cuboid::GetSide(const Dir& dir) const
{
	GLfloat w = halfWidth * scale->x;
	GLfloat h = height * scale->y;
	GLfloat d = halfDepth * scale->z;

	if (scale->x == 0)
	{
		w = 0;
	}
	if (scale->y == 0)
	{
		h = 0;
	}
	if (scale->z == 0)
	{
		d = 0;
	}

	switch (dir)
	{
	case Dir::Left:
		return position->x - w;
	case Dir::Right:
		return position->x + w;
	case Dir::Down:
		return position->y;
	case Dir::Up:
		return position->y + h;
	case Dir::Front:
		return position->z - d;
	case Dir::Back:
		return position->z + d;
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
	const GLfloat w = halfWidth / scale->x;
	const GLfloat h = height / scale->y;
	const GLfloat d = halfDepth / scale->z;

	const GLfloat left = position->x - w + radius;
	const GLfloat right = position->x + w - radius;
	const GLfloat top = position->y + h - radius;
	const GLfloat bottom = position->y - h + radius;
	const GLfloat front = position->z - d + radius;
	const GLfloat back = position->z + d - radius;

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
	const GLfloat w = halfWidth / scale->x;
	const GLfloat h = height / scale->y;
	const GLfloat d = halfDepth / scale->z;

	const GLfloat left = position->x - w;
	const GLfloat right = position->x + w;
	const GLfloat front = position->z - d;
	const GLfloat back = position->z + d;

	if (((rect.left > left && rect.left < right) || (rect.right > left && rect.right < right)) &&
		((rect.top > front && rect.top < back) || (rect.bottom > front && rect.bottom < back)))
	{
		return true;
	}

	return false;
}
GLvoid Cuboid::Draw() const
{
	const GLfloat w = halfWidth * scale->x;
	const GLfloat h = (height/2) * scale->y;
	const GLfloat d = halfDepth * scale->z;

	const Line* identityLine = GetIdentityLine();
	SharedObject line(identityLine);

	// z
	line.SetPosition(*position);
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
	line.SetPosition(*position);
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
	line.SetPosition(*position);
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







Cube::Cube() : ModelObject()
{
	const Model* model = cubeModel;
	ModelObject::LoadModel(model);
}
GLvoid Cube::SetChild(Cube* cube)
{
	cube->SetPivot(&position);
	if (pivot)
	{
		cube->SetPivot(pivot);
	}
	childs.emplace_back(cube);
}
GLvoid Cube::AddPivot(const glm::vec3& pivot)
{
	pivots.emplace_back(pivot);
}
const glm::vec3* Cube::GetPivot(const size_t& index)
{
	::CheckOutOfIndex(index, pivots.size());

	return &pivots[index];
}

GLvoid Cube::Draw() const
{
	IdentityObject::Draw();
	for (Cube* child : childs)
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
	for (Cube* child : childs)
	{
		child->Rotate(pivot, degree);
	}
}
GLvoid Cube::RotateLocal(const glm::vec3& pivot, const GLfloat& degree)
{
	IdentityObject::RotateLocal(pivot, degree);
	for (Cube* child : childs)
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




Ball::Ball() : ModelObject()
{
	const Model* model = sphereModel;
	ModelObject::LoadModel(model);

	GLfloat degreeY = GetRandom<GLfloat>(360);
	GLfloat degreeX = GetRandom<GLfloat>(360);

	vector = Vector3::Back();
	vector = Vector3::RotateOrigin(vector, glm::radians(degreeY), Vector3::Up());
	vector = Vector3::RotateOrigin(vector, glm::radians(degreeX), Vector3::Right());
}
GLvoid Ball::Update()
{
	GLfloat rotationSpeed = 5.0f;
	RotateLocal(Vector3::Up(), vector.x * rotationSpeed);
	RotateLocal(Vector3::Right(), vector.y * rotationSpeed);
	RotateLocal(Vector3::Look(), vector.z * rotationSpeed);

	MoveGlobal({ 0.0f, -0.5f, 0.0f });
	MoveGlobal(vector * speed);
}


static Cube* cubeObject = nullptr;
const Cube* GetIdentityCube()
{
	if (cubeObject == nullptr)
	{
		cubeObject = new Cube();
		cubeObject->SetColor(GRAY);
		cubeObject->BindBuffers();
	}

	return cubeObject;
}
static Line* lineObject = nullptr;
const Line* GetIdentityLine()
{
	if (lineObject == nullptr)
	{
		lineObject = new Line();
		lineObject->SetColor(RED);
		lineObject->BindBuffers();
	}

	return lineObject;
}

static ModelObject* playerObject = nullptr;
const ModelObject* GetIdentityPlayer()
{
	if (playerObject == nullptr)
	{
		playerObject = new ModelObject(playerModel);
		playerObject->SetColor(RED);
		playerObject->BindBuffers();
	}

	return playerObject;
}