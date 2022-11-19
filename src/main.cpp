#include "stdafx.h"
#include "Shader.h"
#include "Object.h"
#include "Player.h"
#include "Model.h"
#include "Timer.h"
#include "Camera.h"
#include "Transform.h"
#include "Map.h"
#include "Light.h"

const Camera* crntCamera = nullptr;
Camera* cameraMain = nullptr;
Camera* cameraFree = nullptr;
Camera* cameraTop = nullptr;
GLboolean isFirstPerson = false;

GLvoid Init();
GLvoid InitMeshes();
GLvoid DrawScene();

GLvoid Update();
GLvoid Mouse(GLint button, GLint state, GLint x, GLint y);
GLvoid MouseMotion(GLint x, GLint y);
GLvoid MousePassiveMotion(GLint x, GLint y);
GLvoid ProcessKeyDown(unsigned char key, GLint x, GLint y);
GLvoid ProcessKeyUp(unsigned char key, GLint x, GLint y);
GLvoid ProcessSpecialKeyDown(GLint key, GLint x, GLint y);

GLvoid ToggleDepthTest();

// values
GLint screenPosX = DEFAULT_SCREEN_POS_X;
GLint screenPosY = DEFAULT_SCREEN_POS_Y;
GLint screenWidth = DEFAULT_SCREEN_WIDTH;
GLint screenHeight = DEFAULT_SCREEN_HEIGHT;
GLboolean isMultiWindow = false;

// world
glm::vec3 worldPosition(0.0f, 0.0f, 0.0f);
glm::vec3 worldRotation(0.0f, 0.0f, 0.0f);

// lights
Light* light = nullptr;
MyColor lightColor;

// objects
vector<ShaderObject*> customObjects;
vector<ShaderObject*> lightObjects;
vector<ShaderObject*> minimapObjects;
Map* crntMap = nullptr;
Player* player = nullptr;

// modes
GLboolean isPersp = true;
GLboolean isCulling = true;
GLboolean isWireFrame = false;

// mouse
GLpoint mouseCenter = { 0,0 };
GLpoint crntPos = { 0,0 };
GLboolean isLeftDown = false;
GLboolean isRightDown = false;

// extern

GLint main(GLint argc, GLchar** argv)
{
	srand((unsigned int)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(DEFAULT_SCREEN_POS_X, DEFAULT_SCREEN_POS_Y);
	glutInitWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glShadeModel(GL_SMOOTH);
	glutCreateWindow("TestProject");

	glewExperimental = GL_TRUE;

	Init();

	glutIdleFunc(Update);
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutSetCursor(GLUT_CURSOR_NONE);
	
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);

	glutPositionFunc(RePosition);
	glutKeyboardFunc(ProcessKeyDown);
	glutKeyboardUpFunc(ProcessKeyUp);
	glutSpecialFunc(ProcessSpecialKeyDown);
	timer::StartUpdate();

	glutMainLoop();
}









///// INIT /////
MyColor backColor;
GLvoid Init()
{
	glewInit();
	InitShader();
	InitMeshes();

	timer::Init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	backColor.SetColor(CYAN);

	cameraFree = new Camera({ 0, 200.0f, 100.0f });
	cameraFree->Look({ 0,0,0 });

	cameraTop = new Camera();
	cameraTop->RotateLocal(89.9f, 0.0f, 0.0f);
	cameraTop->SetPerpective(false);

	cameraMain = cameraFree;
	crntCamera = cameraMain;

	mouseCenter = { screenWidth / 2 + screenPosX, screenHeight / 2 + screenPosY };

	lightColor.SetColor(WHITE);
	ApplyLightColor(lightColor);
}

GLvoid InitMeshes()
{
	constexpr GLfloat lineLength = 5.5f;
	Line* line = nullptr;
	Vector3 vectorLine_1, vectorLine_2;

	// coordinate system lines
	vectorLine_1 = { -lineLength, 0.0f, 0.0f };
	vectorLine_2 = { lineLength, 0.0f, 0.0f };
	line = new Line(vectorLine_1, vectorLine_2);
	line->SetColor(RED);
	line->MoveGlobal({ lineLength, 0, 0 });
	customObjects.emplace_back(line);

	vectorLine_1 = { 0.0f, -lineLength, 0.0f };
	vectorLine_2 = { 0.0f, lineLength, 0.0f };
	line = new Line(vectorLine_1, vectorLine_2);
	line->SetColor(GREEN);
	line->MoveGlobal({ 0, lineLength, 0 });
	customObjects.emplace_back(line);

	vectorLine_1 = { 0.0f, 0.0f, -lineLength };
	vectorLine_2 = { 0.0f, 0.0f, lineLength };
	line = new Line(vectorLine_1, vectorLine_2);
	line->SetColor(BLUE);
	line->MoveGlobal({ 0, 0, lineLength });
	customObjects.emplace_back(line);
	//

	// light test object
	SharedObject* temp = new SharedObject(GetIdentitySphere());
	temp->SetColor(ORANGE);
	temp->SetPosition({ 0, 0, 20 });
	lightObjects.emplace_back(temp);

	// light object
	light = new Light();
	light->SetPosition({ 200, 400, 200 });

	crntMap = new Map();
	player = new Player({ 0,0,0});
}

GLvoid Reset()
{
	for (ShaderObject* object : customObjects)
	{
		delete object;
	}
	for (ShaderObject* object : lightObjects)
	{
		delete object;
	}
	for (ShaderObject* object : minimapObjects)
	{
		delete object;
	}
	customObjects.clear();
	lightObjects.clear();
	minimapObjects.clear();

	delete cameraFree;
	delete cameraTop;
	cameraFree = nullptr;
	cameraTop = nullptr;

	if (crntMap != nullptr)
	{
		delete crntMap;
		crntMap = nullptr;
	}
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	Init();
}








///// Draw /////

GLvoid SetWindow(GLint index)
{
	const GLint halfWidth = screenWidth / 2;
	const GLint halfHeight = screenHeight / 2;
	switch (index)
	{
	case 0:
		crntCamera = cameraMain;
		glViewport(0, 0, screenWidth, screenHeight);
		SetDepthTest(true);
		return;
	case 1:
		crntCamera = cameraTop;
		glViewport(halfWidth + halfWidth/2, halfHeight, halfWidth/2, halfHeight);
		SetDepthTest(false);
		return;
	default:
		assert(0);
	}
}

GLvoid DrawScene()
{
	glClearColor(backColor.r, backColor.g, backColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (isWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT, GL_FILL);
	}

	SetWindow(0);

	Shader crntShader = Shader::Color;
	glUseProgram(GetShaderProgram(crntShader));
	transform::Apply(crntShader, transform::GetView(crntCamera), "viewTransform");
	transform::Apply(crntShader, transform::GetProj(crntCamera), "projTransform");
	for (const ShaderObject* object : customObjects)
	{
		object->Draw();
	}


	crntShader = Shader::Light;
	glUseProgram(GetShaderProgram(crntShader));
	transform::Apply(crntShader, transform::GetView(crntCamera), "viewTransform");
	transform::Apply(crntShader, transform::GetProj(crntCamera), "projTransform");
	SetShader(Shader::Light, "light.pos", light->GetPosition());
	ApplyCameraPos(crntCamera->GetPosition());

	for (const ShaderObject* object : lightObjects)
	{
		object->Draw();
	}

	crntMap->Draw();
		
	if (player != nullptr)
	{
		player->Draw(isFirstPerson);
	}

	light->Draw();

	glBindVertexArray(0);
	glutSwapBuffers();
}


















///// [ HANDLE EVENTS ] /////
GLvoid Update()
{
	timer::CalculateFPS();
	timer::Update();

	// movement
	if (cameraMain == cameraFree)
	{
		constexpr GLfloat cameraMovement = 0.3f;
		GLfloat cameraSpeed = cameraMovement;

		if (IS_KEY_DOWN(KEY_UP))
		{
			cameraFree->MoveZ(cameraSpeed);
		}
		if (IS_KEY_DOWN(KEY_DOWN))
		{
			cameraFree->MoveZ(-cameraSpeed);
		}
		if (IS_KEY_DOWN(KEY_LEFT))
		{
			cameraFree->MoveX(-cameraSpeed);
		}
		if (IS_KEY_DOWN(KEY_RIGHT))
		{
			cameraFree->MoveX(cameraSpeed);
		}
		if (IS_KEY_DOWN(VK_NEXT))
		{
			cameraFree->MoveGlobal({ 0, -cameraSpeed, 0 });
		}
		if (IS_KEY_DOWN(VK_PRIOR))
		{
			cameraFree->MoveGlobal({ 0, cameraSpeed, 0 });
		}
	}
	if (player != nullptr)
	{
		player->Update();
	}

	
	glutPostRedisplay();
}




GLvoid Mouse(GLint button, GLint state, GLint x, GLint y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			isLeftDown = true;
		}
		else if (state == GLUT_UP)
		{
			isLeftDown = false;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			isRightDown = true;
		}
		else if (state == GLUT_UP)
		{
			isRightDown = false;
		}
		break;
	}
}


GLvoid MouseMotion(GLint x, GLint y)
{

}
GLvoid MousePassiveMotion(GLint x, GLint y)
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	crntPos = { cursorPos.x, cursorPos.y };

	GLfloat sensitivity = 10;
	GLfloat dx = (mouseCenter.x - crntPos.x) / sensitivity;
	GLfloat dy = (mouseCenter.y - crntPos.y) / sensitivity;
	
	//d = d * 50.0f;

	if (cameraMain == cameraFree)
	{
		cameraFree->RotateLocal(-dy, dx, 0.0f);
	}
	else if(player != nullptr)
	{
		player->Rotate(0.0f, dx, 0.0f);
	}

	SetCursorPos(mouseCenter.x, mouseCenter.y);
}



// interlock with a control key
static unordered_map<unsigned char, unsigned char> CtrlMap = {
	{23, 'w'},
	{19, 's'},
	{1, 'a'},
	{4, 'd'},
};
GLvoid ProcessKeyDown(unsigned char key, GLint x, GLint y)
{
	if (CtrlMap.find(key) != CtrlMap.end())
	{
		key = CtrlMap[key];
	}

	switch (key)
	{
		// controls
	case '[':
		isMultiWindow = false;
		break;
	case ']':
		isMultiWindow = true;
		break;
	case 'p':
	case 'P':
		cameraMain->SetPerpective(true);
		break;
	case 'o':
	case 'O':
		cameraMain->SetPerpective(false);
		break;
	case 'c':
	case 'C':
		Reset();
		break;
	case 'h':
	case 'H':
		ToggleDepthTest();
		break;

		// camera
	case '1':
		cameraMain = cameraFree;
		isFirstPerson = false;
		break;
	case '2':
		if (player != nullptr)
		{
			cameraMain = player->GetFirstPersonCamera();
			isFirstPerson = true;
		}
		break;
	case '0':
		if (light != nullptr)
		{
			cameraMain->SetPosition(light->GetPosition());
			isFirstPerson = false;
		}
		break;
		// objects

	
		// timers
	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;
	default:
		break;

	}

	if (player != nullptr)
	{
		player->ProcessKeyDown(key);
	}
}
GLvoid ProcessKeyUp(unsigned char key, GLint x, GLint y)
{
	if (CtrlMap.find(key) != CtrlMap.end())
	{
		key = CtrlMap[key];
	}

	if (player != nullptr)
	{
		player->ProcessKeyUp(key);
	}
}
GLvoid ProcessSpecialKeyDown(GLint key, GLint x, GLint y)
{
	// WARNING : GLUT_KEY_LEFT(100) == 'd'(100)
	switch (key)
	{
	case GLUT_KEY_HOME:
		cameraFree->Look({ 0,0,0 });
		break;
	case GLUT_KEY_F1:
		isWireFrame = !isWireFrame;
		break;
	}
}