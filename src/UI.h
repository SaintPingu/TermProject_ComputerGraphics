#include "stdafx.h"

class Player;
class ModelObject;
enum class Textures;

struct UITexture {
	Textures texture;
	glm::vec2 pos = glm::vec2(0, 0);
	GLfloat scale = 0.0f;
};

class UIManager {
private:
	ModelObject* mPlane = nullptr;
	/* Ammo */
	GLint ammo = 0;
	GLint max_ammo = 0;

	UITexture m_max_ammo_Tens;
	UITexture m_max_ammo_Units;

	UITexture m_ammo_Tens;
	UITexture m_ammo_Units;

	UITexture mhp_text;
	UITexture mhp_bar;

	UITexture mslash_text;

	UITexture mgun_symbol;

	const Player* mPlayer = nullptr;

public:
	UIManager();

	GLvoid Draw();
	GLvoid DrawPlane(const UITexture& texture);
	GLvoid SetAmmo(const GLint& num);
	GLvoid SetHP(const GLfloat& HP);
	inline constexpr GLvoid SetPlayer(const Player* player)
	{
		mPlayer = player;
	}
};