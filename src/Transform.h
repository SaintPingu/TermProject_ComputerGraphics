#include "stdafx.h"

class Camera;

namespace transform {
	glm::mat4 GetWorld();
	glm::mat4 GetView(const Camera* camera);
	glm::mat4 GetProj(const Camera* camera);
	GLvoid Apply(const Shader& shader, const glm::mat4& transform, const GLchar* name);
	GLvoid Apply(const Shader& shader, const glm::vec3& vector, const GLchar* name);
	GLvoid Disable(const Shader& shader, const GLchar* name);
}