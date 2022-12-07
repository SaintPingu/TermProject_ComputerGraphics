#include "stdafx.h"

class Player;
class ModelObject;
enum class Textures;;

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
	UITexture mTens;
	UITexture mUnits;

	const Player* mPlayer = nullptr;

public:
	UIManager();

	GLvoid Draw() const;
	GLvoid SetAmmo(const GLint& num);
	inline constexpr GLvoid SetPlayer(const Player* player)
	{
		mPlayer = player;
	}
};