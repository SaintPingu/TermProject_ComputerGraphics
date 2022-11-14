#include "stdafx.h"
#include "Timer.h"
#include "Object.h"
#include "Camera.h"

#define DEFAULT_TIMER_ELAPSE 10

#define EXIT -1
#define INIT -2
#define READY -3


GLboolean CameraRotation_YAxis(GLint value);
GLboolean CameraRotation_Y(GLint value);

static unordered_map<Timer, GLboolean(*)(GLint)> timerTable = {
	{ Timer::WolrdRotation_Y, CameraRotation_YAxis },
	{ Timer::CameraRotation_Y, CameraRotation_Y },
};

static set<GLboolean(*)(GLint)> timers;
static GLboolean isUpdate = false;

// extern
extern Camera* cameraFree;

// init
GLvoid timer::Init()
{
	for (size_t i = 0; i < timerTable.size(); ++i)
	{
		Timer timer = static_cast<Timer>(i);
		InitTimer(timerTable[timer]);
	}

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
	glutTimerFunc(DEFAULT_TIMER_ELAPSE, timer::Update, 0);
}
GLvoid timer::StopUpdate()
{
	isUpdate = false;
}


GLvoid timer::Update(GLint value)
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

	glutTimerFunc(DEFAULT_TIMER_ELAPSE, timer::Update, 0);
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

	cameraFree->RotateYAxis(glm::radians(1.0f * dir));

	return true;
}

GLboolean CameraRotation_X(GLint value)
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

	cameraFree->RotateLocal(glm::radians(1.0f * dir), 0.0f, 0.0f);

	return true;
}
GLboolean CameraRotation_Y(GLint value)
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
		assert(0);
	}

	cameraFree->RotateLocal(0.0f, glm::radians(50.0f * dir), 0.0f);

	return true;
}



