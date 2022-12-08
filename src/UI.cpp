#include "stdafx.h"
#include "UI.h"
#include "Object.h"
#include "Model.h"
#include "Player.h"
#include "Gun.h"

UIManager::UIManager()
{
	const Model* planeModel = GetModel(Models::Plane);
	mPlane = new ModelObject(planeModel, Shader::Back);

	// create max_ammo text
	m_ammo_Tens.texture = Textures::UI_NUM_1;
	m_ammo_Units.texture = Textures::UI_NUM_0;

	m_max_ammo_Tens.pos = glm::vec2(0.8f, -0.5f);
	m_max_ammo_Units.pos = glm::vec2(0.83f, -0.5f);
	m_max_ammo_Tens.scale = 0.005f;
	m_max_ammo_Units.scale = 0.005f;

	// create current_ammo text
	m_ammo_Tens.texture = Textures::UI_NUM_5;
	m_ammo_Units.texture = Textures::UI_NUM_0;

	m_ammo_Tens.pos = glm::vec2(0.7f, -0.5f);
	m_ammo_Units.pos = glm::vec2(0.73f, -0.5f);
	m_ammo_Tens.scale = 0.005f;
	m_ammo_Units.scale = 0.005f;

	// create slash_text
	mslash_text.texture = Textures::UI_TEXT_SLASH;
	mslash_text.pos = glm::vec2(0.765f, -0.5f);
	mslash_text.scale = 0.005f;

	// create HP text
	mhp_text.texture = Textures::UI_TEXT_HP;
	mhp_text.pos = glm::vec2(-0.5f, -0.5f);
	mhp_text.scale = 0.005f;

	// create HP_Bar
	mhp_bar.texture = Textures::UI_COLOR_HP;
	mhp_bar.pos = glm::vec2(-0.6f, -0.5f);
	mhp_bar.scale = 0.005f;


	// create mGun_symble
	mgun_symbol.texture = Textures::UI_SHOTGUN_SYMBOL;
	mgun_symbol.pos = glm::vec2(0.7f, -0.4f);
	mgun_symbol.scale = 0.006f;

}

GLvoid UIManager::DrawPlane(const UITexture& texture)
{
	mPlane->SetPosition(glm::vec3(texture.pos.x, texture.pos.y, 0));
	mPlane->SetTexture(texture.texture);
	mPlane->SetScale(texture.scale);
	mPlane->Draw();
}

GLvoid UIManager::Draw()
{
	
	GLint ammo = mPlayer->GetAmmo();	
	GLint ammo_ten = ammo * 0.1f;
	GLint ammo_Unit = ammo % 10;

	m_ammo_Tens.texture = static_cast<Textures>(static_cast<GLint>(Textures::UI_NUM_0) + ammo_ten);
	m_ammo_Units.texture = static_cast<Textures>(static_cast<GLint>(Textures::UI_NUM_0) + ammo_Unit);

	GLint max_ammo = mPlayer->GetMaxAmmo();
	GLint max_ammo_ten = max_ammo * 0.1f;
	GLint max_ammo_units = max_ammo % 10;

	m_max_ammo_Tens.texture = (static_cast<Textures>(static_cast<GLint>(Textures::UI_NUM_0) + max_ammo_ten));
	m_max_ammo_Units.texture = (static_cast<Textures>(static_cast<GLint>(Textures::UI_NUM_0) + max_ammo_units));

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	DrawPlane(m_max_ammo_Tens);
	DrawPlane(m_max_ammo_Units);
	DrawPlane(m_ammo_Tens);
	DrawPlane(m_ammo_Units);
	DrawPlane(mslash_text);
	DrawPlane(mhp_text);

	GunType gunType = mPlayer->GetGunType();
	switch (gunType)
	{
	case GunType::None:
		mgun_symbol.texture = (static_cast<Textures>(static_cast<GLint>(Textures::UI_SHOTGUN_SYMBOL)));
		break;
	case GunType::Red:
		mgun_symbol.texture = (static_cast<Textures>(static_cast<GLint>(Textures::UI_LAUNCHER_SYMBOL)));
		break;
	case GunType::Blue:
		mgun_symbol.texture = (static_cast<Textures>(static_cast<GLint>(Textures::UI_SHOTGUN_SYMBOL)));
		break;
	case GunType::Green:
		mgun_symbol.texture = (static_cast<Textures>(static_cast<GLint>(Textures::UI_SNIPER_SYMBOL)));
		break;
	case GunType::White:
		break;
	default:
		break;
	}

	DrawPlane(mgun_symbol);


	GLfloat ui_HP = mPlayer->GetHp();

	if (ui_HP > 0) {
		mPlane->SetPosition(glm::vec3(mhp_bar.pos.x + (mhp_bar.scale * ui_HP /4), mhp_bar.pos.y, 0));
		mPlane->SetTexture(mhp_bar.texture);
		mPlane->SetScale(mhp_bar.scale);
		mPlane->SetScaleX(mhp_bar.scale * ui_HP / 20);
		mPlane->Draw();
	}



	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
