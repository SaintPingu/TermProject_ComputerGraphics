#include "stdafx.h"
#include "Map.h"
#include "Model.h"
#include "Object.h"

extern const Model* mapModel;

Map::Map()
{
	object = new ModelObject(mapModel);
	object->SetColor(DARK_GREEN);
	object->BindBuffers();
}

GLvoid Map::Draw()
{
	object->Draw();
	// DrawWireXZ(object->GetBoundings_XZ(), 1.0f); // Draw a wireframe on the floor of the map
}