#pragma once
#include "stdafx.h"

class Cuboid;
enum class Models { Plane = 0, Circle, Cube, LowSphere, GeoSphere, Player, Blooper, GuardTower, _count };
enum class TextureModels { Gun, Map, CubeBackground, _count };
constexpr GLint NUM_OF_MODEL = static_cast<GLint>(Models::_count);
constexpr GLint NUM_OF_TEXTURE_MODEL = static_cast<GLint>(TextureModels::_count);

class Model {
private:
	vector<glm::vec3> mVertices;
	vector<glm::vec3> mNormals;
	vector<glm::vec2> mUVs;
	vector<size_t> mIndices_vertex;
	vector<size_t> mIndices_normal;
	vector<size_t> mIndices_uv;
	
	set<glm::vec2, CompareSet> mVerticesXZ;	// for XZ-collision

	Cuboid* mCuboid = nullptr;				// for 3D-box collision
	GLfloat mWidth = 0;
	GLfloat mHeight = 0;
	GLfloat mDepth = 0;

	GLvoid LoadModel(const GLchar* path);
public:
	Model(const GLchar* path);

	inline set<glm::vec2, CompareSet> GetBoundings_XZ() const { return mVerticesXZ; }

	inline constexpr const vector<glm::vec3>& GetVertices() const { return mVertices; }
	inline constexpr const vector<glm::vec3>& GetNormals() const { return mNormals; }
	inline constexpr const vector<glm::vec2>& GetUVs() const { return mUVs; }
	inline constexpr const vector<size_t>& GetIndices_Vertex() const { return mIndices_vertex; }
	inline constexpr const vector<size_t>& GetIndices_Normal() const { return mIndices_normal; }
	inline constexpr const vector<size_t>& GetIndices_UV() const { return mIndices_uv; }
	inline size_t GetVertexCount() const { return mVertices.size(); }
	inline size_t GetIndexCount() const { return mIndices_vertex.size(); }
	glm::vec3 GetVertex(const size_t& index) const;

	Cuboid* GetCuboid(const glm::vec3* position, const glm::vec3* scale) const;
	inline GLfloat GetWidth() const { return mWidth; }
	inline GLfloat GetHeight() const {return mHeight; }
	inline GLfloat GetDepth() const { return mDepth; }

	GLvoid ReverseNormal();
};

GLvoid InitModels();
const GLchar* GetTexturePath(const TextureModels& textureModel);
const Model* GetModel(const Models& model);
const Model* GetTextureModel(const TextureModels& textureModel);