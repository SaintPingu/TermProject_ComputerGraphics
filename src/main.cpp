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
#include "Bullet.h"
#include "Monster.h"

const Camera* crntCamera = nullptr;
Camera* cameraMain = nullptr;
Camera* cameraFree = nullptr;
Camera* cameraTop = nullptr;
CameraMode cameraMode = CameraMode::Free;

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

// world
glm::vec3 worldPosition(0.0f, 0.0f, 0.0f);
glm::vec3 worldRotation(0.0f, 0.0f, 0.0f);

// lights
BulletManager* bulletManager = nullptr;
MonsterManager* monsterManager = nullptr;
Light* light = nullptr;

// objects
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
	shd::Init();
	InitMeshes();
	timer::Init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	backColor.SetColor(CYAN);

	//********** [ Camera ] **********//
	cameraFree = new Camera({ 0, 200.0f, 100.0f });
	cameraFree->Look({ 0,0,0 });
	cameraFree->SetFovY(110);

	cameraTop = new Camera();
	cameraTop->RotateLocal(89.9f, 0.0f, 0.0f);
	cameraTop->SetPerpective(false);

	cameraMain = cameraFree;
	crntCamera = cameraMain;
	//********************************//

	mouseCenter = { screenWidth / 2 + screenPosX, screenHeight / 2 + screenPosY };

	system("cls");
}

GLvoid InitMeshes()
{
	InitModels();
	InitObjects();
	bulletManager = new BulletManager();
	monsterManager = new MonsterManager();
	monsterManager->AddMonster({ 50, 20, 50 }, MonsterType::Blooper);

	//********** [ Coordinate system lines ] **********//
	constexpr GLfloat lineLength = (20.0f / 2.0f);	// radius = 10
	Line* line = nullptr;
	Vector3 vectorLine_1, vectorLine_2;

	vectorLine_1 = { -lineLength, 0.0f, 0.0f };
	vectorLine_2 = { lineLength, 0.0f, 0.0f };
	line = new Line(vectorLine_1, vectorLine_2);
	line->SetColor(RED);
	line->MoveGlobal({ lineLength, 0, 0 });
	AddObject(Shader::Color, line);

	vectorLine_1 = { 0.0f, -lineLength, 0.0f };
	vectorLine_2 = { 0.0f, lineLength, 0.0f };
	line = new Line(vectorLine_1, vectorLine_2);
	line->SetColor(GREEN);
	line->MoveGlobal({ 0, lineLength, 0 });
	AddObject(Shader::Color, line);

	vectorLine_1 = { 0.0f, 0.0f, -lineLength };
	vectorLine_2 = { 0.0f, 0.0f, lineLength };
	line = new Line(vectorLine_1, vectorLine_2);
	line->SetColor(BLUE);
	line->MoveGlobal({ 0, 0, lineLength });
	AddObject(Shader::Color, line);
	//**************************************************//
	

	// light test object
	SharedObject* temp = new SharedObject(GetIdentitySphere());
	temp->SetColor(ORANGE);
	temp->SetPosition({ 0, 20, 20 });
	AddObject(Shader::Light, temp);

	// light object
	light = new Light();
	light->SetPosition({ 200, 400, 200 });

	crntMap = new Map();
	player = new Player({ 0,0,0 }, &cameraMode);
	monsterManager->SetPlayer(player);
}

GLvoid Reset()
{
	ResetObjects();

	delete bulletManager;
	bulletManager = nullptr;

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
	shd::Use(crntShader);
	shd::SetShader(crntShader, xform::GetView(crntCamera), "viewTransform");
	shd::SetShader(crntShader, xform::GetProj(crntCamera), "projTransform");
	DrawObjects(crntShader);


	crntShader = Shader::Light;
	shd::Use(crntShader);
	shd::SetShader(crntShader, xform::GetView(crntCamera), "viewTransform");
	shd::SetShader(crntShader, xform::GetProj(crntCamera), "projTransform");
	shd::SetShader(Shader::Light, "light.pos", light->GetPviotedPosition());
	shd::SetShader(Shader::Light, "viewPos", crntCamera->GetPviotedPosition());
	DrawObjects(crntShader);
	bulletManager->Draw();
	monsterManager->Draw();

	crntMap->Draw();
		
	if (player != nullptr)
	{
		player->Draw(cameraMode);
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

	if (player != nullptr)
	{
		player->Update();
	}

	bulletManager->Update();
	monsterManager->Update();

	constexpr GLfloat cameraMovement = 100.0f;
	GLfloat cameraSpeed = cameraMovement;

	// movement
	if (cameraMain == cameraFree)
	{
		if (IS_KEY_DOWN(KEY_UP))
		{
			cameraMain->MoveZ(cameraSpeed);
			if (cameraMode == CameraMode::Light)
			{
				light->MoveZ(cameraSpeed);
			}
		}
		if (IS_KEY_DOWN(KEY_DOWN))
		{
			cameraMain->MoveZ(-cameraSpeed);
			if (cameraMode == CameraMode::Light)
			{
				light->MoveZ(-cameraSpeed);
			}
		}
		if (IS_KEY_DOWN(KEY_LEFT))
		{
			cameraMain->MoveX(-cameraSpeed);
			if (cameraMode == CameraMode::Light)
			{
				light->MoveX(-cameraSpeed);
			}
		}
		if (IS_KEY_DOWN(KEY_RIGHT))
		{
			cameraMain->MoveX(cameraSpeed);
			if (cameraMode == CameraMode::Light)
			{
				light->MoveX(cameraSpeed);
			}
		}
		if (IS_KEY_DOWN(VK_NEXT))
		{
			cameraMain->MoveGlobal({ 0, -cameraSpeed, 0 });
			if (cameraMode == CameraMode::Light)
			{
				light->MoveGlobal({ 0, -cameraSpeed, 0 });
			}
		}
		if (IS_KEY_DOWN(VK_PRIOR))
		{
			cameraMain->MoveGlobal({ 0, cameraSpeed, 0 });
			if (cameraMode == CameraMode::Light)
			{
				light->MoveGlobal({ 0, -cameraSpeed, 0 });
			}
		}
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

	if (player != nullptr)
	{
		player->ProcessMouse(button, state, x, y);
	}
}


GLvoid MouseMotion(GLint x, GLint y)
{
	MousePassiveMotion(x, y);
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
		cameraFree->RotateLocal(dy, dx, 0.0f);
		if (cameraMode == CameraMode::Light)
		{
			light->RotateLocal(dy, dx, 0.0f);
		}
	}
	else if(player != nullptr)
	{
		player->Rotate(dy, dx, 0.0f);
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
	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;

		// camera
	case '1':
		cameraMain = cameraFree;
		cameraMode = CameraMode::Free;
		break;
	case '2':
		if (player != nullptr)
		{
			cameraMain = player->GetFirstPersonCamera();
			cameraMode = CameraMode::FirstPerson;
		}
		break;
	case '3':
		if (player != nullptr)
		{
			cameraMain = player->GetThirdPersonCamera();
			cameraMode = CameraMode::ThirdPerson;
		}
		break;
	case '0':
		if (light != nullptr)
		{
			cameraMain = cameraFree;
			cameraMain->SetPosition(light->GetPviotedPosition());
			cameraMain->SetLook(light->GetLook());
			cameraMode = CameraMode::Light;
		}
		break;
		// objects

	
		// timers
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
	// WARNING : (GLUT_KEY_LEFT == 'd') -> 100 //
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