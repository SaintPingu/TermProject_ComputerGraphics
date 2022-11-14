#include "stdafx.h"
#include "Shader.h"
#include "File.h"

static GLint shaderProgram = 0;
static GLint vertexShader = 0;
static GLint fragmentShader = 0;

GLvoid InitShader()
{
	shaderProgram = Make_ShaderProgram();
}
GLint GetShaderProgram()
{
	return shaderProgram;
}

GLvoid Make_VertexShaders()
{
	GLchar* vertexSource = nullptr;
	vertexSource = FileToBuffer("shaders\\vertex.glsl");
	if (vertexSource == nullptr)
	{
		assert(0);
	}

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "[ ERROR ] make vertex shaders\n";
		return;
	}
}
GLvoid Make_FragmentShaders()
{
	GLchar* fragmentSource = nullptr;
	fragmentSource = FileToBuffer("shaders\\fragment.glsl");
	if (fragmentSource == nullptr)
	{
		assert(0);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "[ ERROR ] make fragment shaders\n";
		return;
	}
}

GLint Make_ShaderProgram()
{
	Make_VertexShaders();
	Make_FragmentShaders();

	GLint ShaderProgramID = glCreateProgram();

	glAttachShader(ShaderProgramID, vertexShader);
	glAttachShader(ShaderProgramID, fragmentShader);

	glLinkProgram(ShaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "[ ERROR ] make shader program\n";
		return false;
	}
	glUseProgram(ShaderProgramID);

	return ShaderProgramID;
}