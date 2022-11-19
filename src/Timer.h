#pragma once
#include "stdafx.h"

enum class Timer : GLint {
	WolrdRotation_Y = 0,
};

namespace timer {
	GLvoid Init();
	GLvoid InitTimer(const Timer& timer);
	GLvoid InitTimer(GLboolean(*timerFunc)(GLint));
	GLvoid ReadyTimer(const Timer& timer);

	GLvoid StartUpdate();
	GLvoid StopUpdate();
	GLvoid Update();

	GLvoid ToggleTimer(const Timer& timer);
	GLvoid EnableTimer(const Timer& timer);
	GLvoid DisableTimer(const Timer& timer);


	GLvoid ToggleTimer(GLboolean(*timerFunc)(GLint));
	GLvoid EnableTimer(GLboolean(*timerFunc)(GLint));
	GLvoid DisableTimer(GLboolean(*timerFunc)(GLint));

	GLvoid CalculateFPS();
	GLfloat DeltaTime();
}

