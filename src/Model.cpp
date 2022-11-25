#include "stdafx.h"
#include "model.h"
#include "object.h"

const Model* mapModel = new Model("obj\\map3.obj");
const Model* playerModel = new Model("obj\\player.obj");
const Model* cubeModel = new Model("obj\\cube.obj");
const Model* sphereModel = new Model("obj\\GeoSphere.obj");
const Model* circleModel = new Model("obj\\circle.obj");

const Model* gunModel = new Model("obj\\gun.obj");

Model::Model(const GLchar* path)
{
	LoadModel(path);
}
GLvoid Model::LoadModel(const GLchar* path)
{
	FILE* objFile = fopen(path, "r");
	if (objFile == nullptr)
	{
		printf("Can't load objFile : %s", path);
		assert(0);
		return;
	}
	printf("Load Model : %s\n", path);

	GLfloat left = FLOAT_MAX;
	GLfloat right = FLOAT_MIN;

	GLfloat bottom = FLOAT_MAX;
	GLfloat top = FLOAT_MIN;

	GLfloat front = FLOAT_MAX;
	GLfloat back = FLOAT_MIN;

	GLchar data[128];
	while (feof(objFile) == false)
	{
		fscanf(objFile, "%s", data);

		if (strcmp(data, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.y < bottom)
			{
				bottom = vertex.y;
			}
			else if (vertex.y > top)
			{
				top = vertex.y;
			}

			if (vertex.x < left)
			{
				left = vertex.x;
			}
			else if (vertex.x > right)
			{
				right = vertex.x;
			}

			if (vertex.z < front)
			{
				front = vertex.z;
			}
			else if (vertex.z > back)
			{
				back = vertex.z;
			}

			mVertices.emplace_back(vertex);

			vertex.x = round(vertex.x * 100) / 100.0f;
			vertex.z = round(vertex.z * 100) / 100.0f;
			mVerticesXZ.insert(glm::vec2(vertex.x, vertex.z));
		}
		else if (strcmp(data, "vt") == 0)
		{
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			mUVs.emplace_back(normal);
		}
		else if (strcmp(data, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			mNormals.emplace_back(normal);
		}
		else if (strcmp(data, "f") == 0)
		{
			size_t vertexIndex[3], uvIndex[3], normalIndex[3];
			for (size_t i = 0; i < 3; ++i)
			{
				fscanf(objFile, "%d/%d/%d", &vertexIndex[i], &uvIndex[i], &normalIndex[i]);
			}

			for (size_t i = 0; i < 3; ++i)
			{
				mVertexIndices.emplace_back(vertexIndex[i] - 1);
				mUVIndices.emplace_back(uvIndex[i] - 1);
				mNormalIndices.emplace_back(normalIndex[i] - 1);
			}
		}
	}
	mWidth = right - left;
	mHeight = top - bottom;
	mDepth = back - front;
}



const vector<glm::vec3>& Model::GetVertices() const
{
	return mVertices;
}
const vector<glm::vec3>& Model::GetNormals() const
{
	return mNormals;
}
size_t Model::GetVertexCount() const
{
	return mVertices.size();
}
const vector<size_t>& Model::GetIndices() const
{
	return mVertexIndices;
}
const vector<size_t>& Model::GetNormalIndices() const
{
	return mNormalIndices;
}
size_t Model::GetIndexCount() const
{
	return mVertexIndices.size();
}
glm::vec3 Model::GetVertex(const size_t& index) const
{
	assert(index <= mVertices.size());

	return mVertices[index];
}

Cuboid* Model::GetCuboid(const glm::vec3* position, const glm::vec3* scale) const
{
	return new Cuboid(position, scale, mWidth, mHeight, mDepth);
}