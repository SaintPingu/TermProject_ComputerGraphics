#include "stdafx.h"


GLvoid InitShader();
GLint GetShaderProgram(const Shader& shader);
GLvoid ApplyLightPos(const glm::vec3& lightPos, const glm::mat4& transform = glm::mat4(1.0f));
GLvoid ApplyLightColor(const glm::vec3& color);
GLvoid ApplyCameraPos(const glm::vec3& cameraPos);
GLvoid ApplyObjectColor(const glm::vec3& color);
