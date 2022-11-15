#include "stdafx.h"

#define PLAYER_RADIUS 5 // Width = 10 : 1m -> radius = 0.5m
// #define PLAYER_WALK_SPEED 10 // 1 m/s
#define PLAYER_WALK_SPEED 50 // for test
#define PLAYER_JUMP_SPEED 30 // 3 m/s

class SharedObject;
class Camera;
class Cuboid;
class Triangle;
class Player;
class Circle;

namespace playerState {
	class PlayerState abstract {
	protected:
		Player* player = nullptr;
	public:
		PlayerState(Player* player)
		{
			this->player = player;
		}
		virtual GLvoid Enter(const Event& e = Event::None, const GLint& value = 0) abstract;
		virtual GLvoid Exit() abstract;
		virtual GLvoid Update() abstract;
		virtual GLvoid HandleEvent(const Event& e, const GLint& key) abstract;
	};

	class Idle : public PlayerState {
	public:
		Idle(Player* player) : PlayerState(player) {};
		GLvoid Enter(const Event& e = Event::None, const GLint& value = 0) override;
		GLvoid Exit() override;
		GLvoid Update() override;
		GLvoid HandleEvent(const Event& e, const GLint& key) override;
	};

	class Walk : public PlayerState {
	public:
		Walk(Player* player) : PlayerState(player) {};
		GLvoid Enter(const Event& e = Event::None, const GLint& value = 0) override;
		GLvoid Exit() override;
		GLvoid Update() override;
		GLvoid HandleEvent(const Event& e, const GLint& key) override;
	};

	class Jump : public PlayerState {
	private:
		const GLfloat jumpTime = 0.5f; // jump up N sec
		GLfloat t = 0;

		GLboolean isKeyUp = false;
	public:
		Jump(Player* player) : PlayerState(player) {};
		GLvoid Enter(const Event& e = Event::None, const GLint& value = 0) override;
		GLvoid Exit() override;
		GLvoid Update() override;
		GLvoid HandleEvent(const Event& e, const GLint& key) override;
	};
}



class Player {
private:
	playerState::PlayerState* crntState = nullptr;
	GLchar dirX = 0;
	GLchar dirY = 0;
	GLchar dirZ = 0;

	glm::vec3 position = { 0, 0, 0 };
	glm::vec3 tpCameraPosition = { 0, 0, 0 };
	GLfloat tpCameraPitch = 0.0f;


	SharedObject* object = nullptr;
	Camera* fpCamera = nullptr;
	Camera* tpCamera = nullptr;

	Triangle* icon = nullptr;

	GLfloat speed = PLAYER_WALK_SPEED;
	GLfloat jumpSpeed = PLAYER_JUMP_SPEED;

	GLfloat floor = 0.0f;
	GLfloat top = 0.0f;
	GLfloat yTop = 0.0f;
	GLfloat yBot = 0.0f;

	Circle* boundingCircle = nullptr;

public:
	Player(const glm::vec3& position);
	~Player();

	Cuboid* cuboid = nullptr;

	// state
	enum class State { Idle = 0, Walk, Run, Jump };
	GLvoid ChangeState(const State& playerState, const Event& e = Event::None, const GLint& value = 0);

	// Frame
	GLvoid Update();
	GLvoid Draw() const;
	GLvoid DrawIcon() const;

	// Process
	GLvoid ProcessKeyDown(const GLint& key);
	GLvoid ProcessKeyUp(const GLint& key);

	// Movement
	GLvoid Move();
	GLvoid Stop();
	GLvoid AddDir(const GLint& key);
	GLvoid SubDir(const GLint& key);
	GLvoid SetDir(const GLint& key, const GLint& value);
	inline constexpr GLchar GetDirX() const { return dirX; }
	inline constexpr GLchar GetDirY() const { return dirY; }
	inline constexpr GLchar GetDirZ() const { return dirZ; }

	// Rotation
	GLvoid Rotate(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll);

	// Variables
	glm::vec3 GetPosition() const;
	inline constexpr Camera* GetFirstPersonCamera()
	{
		return fpCamera;
	}
	inline constexpr Camera* GetThirdPersonCamera()
	{
		return tpCamera;
	}
};