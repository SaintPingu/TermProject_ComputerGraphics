#include "stdafx.h"

class Camera;

namespace transform {
	GLvoid Disable(const GLchar* name);
	glm::mat4 GetWorld();
	glm::mat4 GetView(const Camera* camera);
	glm::mat4 GetProj(const Camera* camera);
	GLvoid Apply(const glm::mat4& transform, const GLchar* name);

	GLvoid ApplyColor(const glm::vec3& color);
	GLvoid DisableColor();
}

