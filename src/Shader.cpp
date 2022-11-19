#include "stdafx.h"
#include "Shader.h"
#include "File.h"

static GLint shaders[NUM_OF_SHADER];

GLint Make_ShaderProgram(const Shader& shader);
GLint Make_VertexShaders(const string& name);
GLint Make_FragmentShaders(const string& name);

GLvoid InitShader()
{
	for (GLsizei i = 0; i < NUM_OF_SHADER; ++i)
	{
		const Shader shader = static_cast<Shader>(i);
		GLint* shaderProgram = &shaders[i];
		*shaderProgram = Make_ShaderProgram(shader);
	}
}
GLint GetShaderProgram(const Shader& shader)
{
	return shaders[static_cast<GLint>(shader)];
}

GLint Make_ShaderProgram(const Shader& shader)
{
	string vertexShaderName;
	string fragShaderName;
	switch (shader)
	{
	case Shader::Color:
		vertexShaderName = "vertex.glsl";
		fragShaderName = "fragment.glsl";
		break;
	case Shader::Light:
		vertexShaderName = "light_vertex.glsl";
		fragShaderName = "light_fragment.glsl";
		break;
	default:
		assert(0);
		break;
	}

	GLint vertexShader = Make_VertexShaders(vertexShaderName);
	GLint fragShader = Make_FragmentShaders(fragShaderName);

	GLint ShaderProgramID = glCreateProgram();

	glAttachShader(ShaderProgramID, vertexShader);
	glAttachShader(ShaderProgramID, fragShader);

	glLinkProgram(ShaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "[ ERROR ] make shader program\n";
		printf("[ error :: shader program ]%s\n", errorLog);
		assert(0);
	}

	return ShaderProgramID;
}

GLint Make_VertexShaders(const string& name)
{
	GLchar* vertexSource = nullptr;

	const string path = "E:\\University\\2-2\\Computer Graphics\\Practices\\22\\" + name;

	vertexSource = FileToBuffer(path.c_str());
	if (vertexSource == nullptr)
	{
		assert(0);
	}
	cout << "Load Vertex Shader: " << path << endl;

	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "[ ERROR ] make vertex shaders\n";
		printf("Vertex Error : %s\n", errorLog);
		printf("[ error :: vertex ]%s\n", errorLog);
		assert(0);
	}

	return vertexShader;
}
GLint Make_FragmentShaders(const string& name)
{
	GLchar* fragmentSource = nullptr;
	const string path = "E:\\University\\2-2\\Computer Graphics\\Practices\\22\\" + name;

	fragmentSource = FileToBuffer(path.c_str());
	if (fragmentSource == nullptr)
	{
		assert(0);
	}
	cout << "Load Fragment Shader : " << path << endl;

	GLint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, (const GLchar**)&fragmentSource, NULL);
	glCompileShader(fragShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, errorLog);
		std::cerr << "[ ERROR ] make fragment shaders\n";
		printf("Fragment Error : %s\n", errorLog);
		printf("[ error :: fragment ]%s\n", errorLog);
		assert(0);
	}

	return fragShader;
}


GLvoid ApplyLightColor(const glm::vec3& color)
{
	unsigned int lightPosLocation = glGetUniformLocation(GetShaderProgram(Shader::Light), "light.color");
	glUniform3f(lightPosLocation, color.r, color.g, color.b);
}
GLvoid ApplyLightColorRef(const COLORREF& color)
{
	MyColor c(color);
	ApplyLightColor(c);
}

GLvoid ApplyCameraPos(const glm::vec3& cameraPos)
{
	unsigned int lightPosLocation = glGetUniformLocation(GetShaderProgram(Shader::Light), "viewPos");
	glUniform3f(lightPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);
}
GLvoid ApplyObjectColor(const Shader& shader, const glm::vec3& color)
{
	unsigned int objColorLocation = glGetUniformLocation(GetShaderProgram(shader), "objectColor");
	glUniform3f(objColorLocation, color.r, color.g, color.b);
}


GLvoid SetShader(const Shader& shader, const GLchar* name, const glm::vec3& value)
{
	unsigned int location = glGetUniformLocation(GetShaderProgram(Shader::Light), name);
	glUniform3f(location, value.x, value.y, value.z);
}
GLvoid SetShader(const Shader& shader, const GLchar* name, const GLfloat& value)
{
	unsigned int location = glGetUniformLocation(GetShaderProgram(Shader::Light), name);
	glUniform1f(location, value);
}