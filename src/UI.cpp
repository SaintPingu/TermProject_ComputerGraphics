#include "stdafx.h"
#include "UI.h"
#include "Object.h"
#include "Model.h"
#include "Player.h"

UIManager::UIManager()
{
	const Model* planeModel = GetModel(Models::Plane);
	mPlane = new ModelObject(planeModel, Shader::Back);

	mTens.texture = Textures::UI_NUM_5;
	mUnits.texture = Textures::UI_NUM_0;

	mTens.pos = glm::vec2(0.5f, 0.5f);
	mUnits.pos = glm::vec2(-0.5f, -0.5f);

	mTens.scale = 0.05f;
	mUnits.scale = 0.05f;
}

GLvoid UIManager::Draw() const
{
	GLint ammo = mPlayer->GetAmmo();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
