#pragma once
#include "stdafx.h"

class Cuboid;
enum class Models { Circle = 0, Cube, LowSphere, GeoSphere, Map, Player, Gun, Blooper, GuardTower, _count };
constexpr GLint NUM_OF_MODELS = static_cast<GLint>(Models::_count);

GLvoid InitModels();

class Model {
private:
	vector<glm::vec3> mVertices;
	vector<glm::vec3> mUVs;
	vector<glm::vec3> mNormals;
	vector<size_t> mVertexIndices;
	vector<size_t> mUVIndices;
	vector<size_t> mNormalIndices;
	
	set<glm::vec2, CompareSet> mVerticesXZ;	// for XZ-collision

	Cuboid* mCuboid = nullptr;				// for 3D-box collision
	GLfloat mWidth = 0;
	GLfloat mHeight = 0;
	GLfloat mDepth = 0;
public:
	Model(const GLchar* path);
	GLvoid LoadModel(const GLchar* path);

	inline set<glm::vec2, CompareSet> GetBoundings_XZ() const { return mVerticesXZ; }

	const vector<glm::vec3>& GetVertices() const;
	const vector<glm::vec3>& GetNormals() const;
	const vector<size_t>& GetIndices() const;
	const vector<size_t>& GetNormalIndices() const;
	size_t GetVertexCount() const;
	size_t GetIndexCount() const;
	glm::vec3 GetVertex(const size_t& index) const;

	Cuboid* GetCuboid(const glm::vec3* position, const glm::vec3* scale) const;
	inline GLfloat GetWidth() const { return mWidth;}
	inline GLfloat GetHeight() const {return mHeight; }
	inline GLfloat GetDepth() const { return mDepth; }
};

const Model* GetModel(const Models& model);