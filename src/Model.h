#include "stdafx.h"

class Cuboid;

class Model {
private:
	vector<glm::vec3> vertices;
	vector<glm::vec3> uvs;
	vector<glm::vec3> normals;
	vector<size_t> vertexIndices;

	Cuboid* cuboid = nullptr;
	GLfloat width = 0;
	GLfloat height = 0;
	GLfloat depth = 0;
public:
	Model(const GLchar* path);
	GLvoid LoadModel(const GLchar* path);

	const vector<glm::vec3>& GetVertices() const;
	size_t GetVertexCount() const;
	const vector<size_t>& GetIndices() const;
	size_t GetIndexCount() const;
	glm::vec3 GetVertex(const size_t& index) const;

	Cuboid* GetCuboid(const glm::vec3* position, const glm::vec3* scale) const;
	inline GLfloat GetWidth() const
	{
		return width;
	}
	inline GLfloat GetHeight() const
	{
		return height;
	}
	inline GLfloat GetDepth() const
	{
		return depth;
	}
};