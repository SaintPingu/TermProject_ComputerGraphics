#pragma once
#include "Object.h"

class Light : public Object{
private:
	SharedObject* object = nullptr;
public:
	Light();
	GLvoid Draw() const;
};