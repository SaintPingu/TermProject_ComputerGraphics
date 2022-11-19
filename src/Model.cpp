#include "stdafx.h"
#include "model.h"
#include "object.h"

const Model* mapModel = new Model("obj\\map3.obj");
const Model* playerModel = new Model("obj\\player.obj");
const Model* cubeModel = new Model("obj\\cube.obj");
const Model* sphereModel = new Model("obj\\GeoSphere.obj");
const Model* circleModel = new Model("obj\\circle.obj");

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

			vertices.emplace_back(vertex);

			// process of floating point precision error
			// Bug : only insert half vertices
			vertex.x = round(vertex.x * 100) / 100.0f;
			vertex.z = round(vertex.z * 100) / 100.0f;
			verticesXZ.insert(glm::vec2(vertex.x, vertex.z));
		}
		else if (strcmp(data, "vt") == 0)
		{
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			uvs.emplace_back(normal);
		}
		else if (strcmp(data, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.emplace_back(normal);
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
				vertexIndices.emplace_back(vertexIndex[i] - 1);
			}
		}
		/*else if (strcmp(data, "f") == 0)
		{
			size_t vertexIndex[3], uvIndex[3], normalIndex[3];
			for (size_t i = 0; i < 3; ++i)
			{
				fscanf(objFile, "%d//%d", &vertexIndex[i], &normalIndex[i]);
			}

			for (size_t i = 0; i < 3; ++i)
			{
				vertexIndices.emplace_back(vertexIndex[i] - 1);
			}
		}*/
	}
	width = right - left;
	height = top - bottom;
	depth = back - front;
}
//GLvoid LoadModel(const GLchar* path)
//{
//	ifstream file(path);
//	if (file.fail())
//	{
//		printf("Can't load objFile : %s", path);
//		assert(0);
//		return;
//	}

//	string line;
//	while (std::getline(file, line))
//	{
//		string text;
//		istringstream iss(line);

//		iss >> text;

//		if (text == "v")
//		{
//			glm::vec4 vertex;
//			iss >> vertex.x;
//			iss >> vertex.y;
//			iss >> vertex.z;
//			vertices.emplace_back(vertex);
//		}
//		else if (text == "vt")
//		{
//			glm::vec4 uv;
//			iss >> uv.x;
//			iss >> uv.y;
//			iss >> uv.z;
//			uvs.emplace_back(uv);
//		}
//		else if (text == "vn")
//		{
//			glm::vec4 normal;
//			iss >> normal.x;
//			iss >> normal.y;
//			iss >> normal.z;
//			normals.emplace_back(normal);
//		}
//		else if (text == "f")
//		{
//			size_t vertexIndex[3], uvIndex[3], normalIndex[3];
//			for (size_t i = 0; i < 3; ++i)
//			{
//				iss >> vertexIndex[i];
//				iss >> uvIndex[i];
//				iss >> normalIndex[i];
//			}

//			for (size_t i = 0; i < 3; ++i)
//			{
//				vertexIndices.emplace_back(vertexIndex[i] - 1);
//			}
//		}
//	}
//}



const vector<glm::vec3>& Model::GetVertices() const
{
	return vertices;
}
const vector<glm::vec3>& Model::GetNormals() const
{
	return normals;
}
size_t Model::GetVertexCount() const
{
	return vertices.size();
}
const vector<size_t>& Model::GetIndices() const
{
	return vertexIndices;
}
size_t Model::GetIndexCount() const
{
	return vertexIndices.size();
}
glm::vec3 Model::GetVertex(const size_t& index) const
{
	assert(index <= vertices.size());

	return vertices[index];
}

Cuboid* Model::GetCuboid(const glm::vec3* position, const glm::vec3* scale) const
{
	return new Cuboid(position, scale, width, height, depth);
}