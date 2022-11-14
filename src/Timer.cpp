#include "stdafx.h"
#include "Timer.h"
#include "Object.h"
#include "Camera.h"

#define EXIT -1
#define INIT -2
#define READY -3


GLboolean CameraRotation_YAxis(GLint value);

static unordered_map<Timer, GLboolean(*)(GLint)> timerTable = {
	{ Timer::WolrdRotation_Y, CameraRotation_YAxis },
};

static set<GLboolean(*)(GLint)> timers;
static GLboolean isUpdate = false;



// fps
static GLfloat deltaTime = 0;
static DWORD lastTime = 0;
static DWORD fps_lastTime = 0;
GLvoid timer::CalculateFPS()
{
	static GLfloat fps = 0;
	static size_t frameCount = 0;
	static DWORD crntTime = 0;

	++frameCount;

	crntTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (crntTime - lastTime) / 1000.0f;

	GLint timeInterval = crntTime - fps_lastTime;

	if (timeInterval > 1000)
	{
		fps = frameCount / (timeInterval / 1000.0f);

		frameCount = 0;

		fps_lastTime = crntTime;
	}

	lastTime = crntTime;
	COORD cursor = { 0,0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
	printf("fps : %.3f\n", fps);
	printf("delta time : %lf\n", deltaTime);

	//printf("%d\n", fp);
}
GLfloat timer::DeltaTime()
{
	return deltaTime;
}


// init
GLvoid timer::Init()
{
	for (size_t i = 0; i < timerTable.size(); ++i)
	{
		Timer timer = static_cast<Timer>(i);
		InitTimer(timerTable[timer]);
	}

	lastTime = glutGet(GLUT_ELAPSED_TIME);
	fps_lastTime = glutGet(GLUT_ELAPSED_TIME);

	timers.clear();
}
GLvoid timer::InitTimer(const Timer& timer)
{
	timer::InitTimer(timerTable[timer]);
}
GLvoid timer::InitTimer(GLboolean(*timerFunc)(GLint))
{
	timerFunc(INIT);
	
	if (timers.find(timerFunc) != timers.end())
	{
		timers.erase(timerFunc);
	}
}
GLvoid timer::ReadyTimer(const Timer& timer)
{
	timerTable[timer](READY);
}

// update
GLvoid timer::StartUpdate()
{
	if (isUpdate)
	{
		return;
	}

	isUpdate = true;
}
GLvoid timer::StopUpdate()
{
	isUpdate = false;
}


GLvoid timer::Update()
{
	if (isUpdate == false)
	{
		return;
	}

	vector<GLboolean(*)(GLint)> exitTimers;
	exitTimers.reserve(timers.size());

	for (GLboolean(*timerFunc)(GLint) : timers)
	{
		if (timerFunc(0) == false)
		{
			exitTimers.emplace_back(timerFunc);
		}
	}
	for (GLboolean(*timerFunc)(GLint) : exitTimers)
	{
		timerFunc(EXIT);
		timers.erase(timerFunc);
	}
}



// control
GLvoid timer::ToggleTimer(const Timer& timer)
{
	timer::ToggleTimer(timerTable[timer]);
}
GLvoid timer::EnableTimer(const Timer& timer)
{
	timer::EnableTimer(timerTable[timer]);
}
GLvoid timer::DisableTimer(const Timer& timer)
{
	timer::DisableTimer(timerTable[timer]);
}

GLvoid timer::ToggleTimer(GLboolean(*timerFunc)(GLint))
{
	if (timers.find(timerFunc) != timers.end())
	{
		timerFunc(EXIT);
		timers.erase(timerFunc);
	}
	else
	{
		timers.insert(timerFunc);
	}
}
GLvoid timer::EnableTimer(GLboolean(*timerFunc)(GLint))
{
	if (timers.find(timerFunc) == timers.end())
	{
		timers.insert(timerFunc);
	}
}
GLvoid timer::DisableTimer(GLboolean(*timerFunc)(GLint))
{
	if (timers.find(timerFunc) != timers.end())
	{
		timerFunc(EXIT);
		timers.erase(timerFunc);
	}
}



// timers
GLboolean CameraRotation_YAxis(GLint value)
{
	static GLint dir = 0;
	switch (value)
	{
	case 0:
		break;
	case INIT:
		dir = LEFT;
		return false;
	case EXIT:
		dir *= -1;
		return false;
	default:
		break;
	}

	return true;
}
