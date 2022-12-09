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
	mHP.texture = Textures::UI_TEXT_HP;

	mTens.pos = glm::vec2(0.65f, -0.5f);
	mUnits.pos = glm::vec2(0.7f, -0.5f);
	mHP.pos = glm::vec2(-0.7f, -0.5f);

	mHP.scale = 0.01f;
	mTens.scale = 0.01f;
	mUnits.scale = 0.01f;
}
GLvoid UIManager::Draw()
{
	GLint ammo = mPlayer->GetAmmo();
	
	GLint ammo_ten = static_cast<GLint>(ammo * 0.1f);
	GLint ammo_Unit = ammo % 10;

	mTens.texture = static_cast<Textures>(static_cast<GLint>(Textures::UI_NUM_0) + ammo_ten);
	mUnits.texture = static_cast<Textures>(static_cast<GLint>(Textures::UI_NUM_0) + ammo_Unit);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	mPlane->SetPosition(glm::vec3(mTens.pos.x, mTens.pos.y, 0));
	mPlane->SetTexture(mTens.texture);
	mPlane->SetScale(mTens.scale);
	mPlane->Draw();

	mPlane->SetPosition(glm::vec3(mUnits.pos.x, mUnits.pos.y, 0));
	mPlane->SetTexture(mUnits.texture);
	mPlane->SetScale(mUnits.scale);
	mPlane->Draw();

	mPlane->SetPosition(glm::vec3(mHP.pos.x, mHP.pos.y, 0));
	mPlane->SetTexture(mHP.texture);
	mPlane->SetScale(mHP.scale);
	mPlane->Draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
