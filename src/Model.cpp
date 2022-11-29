#pragma warning(disable: 6031)	// ��ȯ���� ���õǾ����ϴ�. 'fscanf'
#pragma warning(disable: 6054)	// �������� ���ڿ��� 0���� ������� ���� �� �ֽ��ϴ�.
#include "stdafx.h"
#include "model.h"
#include "object.h"

// model
static const Model* circleModel = nullptr;
static const Model* cubeModel = nullptr;
static const Model* lowSphereModel = nullptr;
static const Model* sphereModel = nullptr;
static const Model* mapModel = nullptr;

// player
static const Model* playerModel = nullptr;
static const Model* gunModel = nullptr;

// monster
static const Model* blooperModel = nullptr;

// building
static const Model* guardTowerModel = nullptr;

unordered_map<Models, const Model*> modelMap{
	{Models::Circle, circleModel},
	{Models::Cube, cubeModel},
	{Models::LowSphere, lowSphereModel},
	{Models::GeoSphere, sphereModel},
	{Models::Map, mapModel},
	{Models::Player, playerModel},
	{Models::Gun, gunModel},
	{Models::Blooper, blooperModel},
	{Models::GuardTower, guardTowerModel},
};

GLvoid InitModels()
{
	modelMap[Models::Circle] = new Model("obj\\circle.obj");
	modelMap[Models::Cube] = new Model("obj\\cube.obj");
	modelMap[Models::LowSphere] = new Model("obj\\low_sphere.obj");
	modelMap[Models::GeoSphere] = new Model("obj\\geo_sphere.obj");
	modelMap[Models::Map] = new Model("obj\\map.obj");

	modelMap[Models::Player] = new Model("obj\\player.obj");
	modelMap[Models::Gun] = new Model("obj\\gun.obj");

	modelMap[Models::Blooper] = new Model("obj\\blooper.obj");;
	modelMap[Models::GuardTower] = new Model("obj\\guard_tower3.obj");;
}

const Model* GetModel(const Models& model)
{
	const Model* result = modelMap[model];
	assert(result != nullptr);

	return result;
}

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

	set<size_t> set_overlapChecker_vertex;
	set<pair<size_t, size_t>> set_overlapChecker_pair;

	// ���� element reading ��, ���� element�� overlapCount��ŭ vertex index�� �������Ѿ� ��.
	size_t overlapCount = 0;
	size_t beforeOverlapCount = 0;

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
			glm::vec3 uv;
			fscanf(objFile, "%f %f %f\n", &uv.x, &uv.y, &uv.z);
			mUVs.emplace_back(glm::vec2(uv.x, uv.y));
		}
		else if (strcmp(data, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			mNormals.emplace_back(normal);
		}
		else if (strcmp(data, "f") == 0)
		{
			size_t indices_vertex[3], indices_uv[3], indices_normal[3];
			for (size_t i = 0; i < 3; ++i)
			{
				fscanf(objFile, "%d/%d/%d", &indices_vertex[i], &indices_uv[i], &indices_normal[i]);
				indices_vertex[i] += beforeOverlapCount;
			}

			// [ �ϳ��� vertex�� ���� noraml, texture ���� ] : �� vertex���� normal, texture ������ 1:1 �����ؾ� �� -> ������ ���� �߰��ؾ���
			// 
			// vertex index�� �ߺ��� ���
			// if	: normal index�� ����	-> ���� normal ������� ���� X		( pair("vertex Index", "normal index") �ߺ� �˻� )
			// else : �ٸ� normal ���		-> ���� �߰� �ʿ�
			//								-> ����, vertices�� vertex index�� �ִ� �� emplace_back, vertex index �� ������ �ε���[mVertices.size()]�� ����

			for (size_t i = 0; i < 3; ++i)
			{
				const size_t index_vertex = indices_vertex[i];

				// vertex index �ߺ� �˻�
				if (set_overlapChecker_vertex.find(index_vertex) == set_overlapChecker_vertex.end())
				{
					continue;
				}

				// normal index �ߺ� �˻�
				const size_t index_normal = indices_normal[i];
				if (set_overlapChecker_pair.find(make_pair(index_vertex, index_normal)) == set_overlapChecker_pair.end())
				{
					continue;	// �� vertex�� ���� normal ���
				}

				// �� vertex�� �ٸ� normal ���

				const glm::vec3 newVertex = mVertices[index_vertex - 1];	// ���� index�� �ִ� vertex ����
				mVertices.emplace_back(newVertex);							// ������ ���ҿ� �߰�
				indices_vertex[i] = mVertices.size();						// index�� ������ ���ҷ� ����
				++overlapCount;
			}
			

			for (size_t i = 0; i < 3; ++i)
			{
				set_overlapChecker_vertex.insert(indices_vertex[i]);
				set_overlapChecker_pair.insert(make_pair(indices_vertex[i], indices_normal[i]));

				mIndices_vertex.emplace_back(indices_vertex[i] - 1);
				mIndices_uv.emplace_back(indices_uv[i] - 1);
				mIndices_normal.emplace_back(indices_normal[i] - 1);
			}
		}
		else if (strcmp(data, "o") == 0)
		{
			// ���ο� object element -> set_overlapChecker �ʱ�ȭ
			set_overlapChecker_vertex.clear();
			set_overlapChecker_pair.clear();
			beforeOverlapCount += overlapCount;
			overlapCount = 0;
		}
	}
	mWidth = right - left;
	mHeight = top - bottom;
	mDepth = back - front;
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