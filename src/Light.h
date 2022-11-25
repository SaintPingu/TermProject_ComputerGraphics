#pragma once
#include "Object.h"

class Light : public Object{
private:
	SharedObject* mObject = nullptr;
public:
	Light();
	GLvoid Draw() const;
};