#include "stdafx.h"


GLvoid InitShader();
GLint GetShaderProgram(const Shader& shader);
GLvoid ApplyLightColor(const glm::vec3& color);
GLvoid ApplyLightColorRef(const COLORREF& color);
GLvoid ApplyCameraPos(const glm::vec3& cameraPos);
GLvoid ApplyObjectColor(const glm::vec3& color);

GLvoid SetShader(const Shader& shader, const GLchar* name, const glm::vec3& value);
GLvoid SetShader(const Shader& shader, const GLchar* name, const GLfloat& value);
