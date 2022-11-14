#include "stdafx.h"

class SharedObject;
class Camera;
class Cuboid;
class Triangle;
class Player;


namespace playerState {
	class PlayerState abstract {
	protected:
		Player* player = nullptr;
	public:
		virtual GLvoid Enter(Player* player, const Event& e = Event::None, const unsigned char& value = 0)
		{
			this->player = player;
		}
		virtual GLvoid Exit()
		{
			this->player = nullptr;
		}
		virtual GLvoid Update() abstract;
		virtual GLvoid HandleKeyDown(const unsigned char& key) abstract;
		virtual GLvoid HandleKeyUp(const unsigned char& key) abstract;
	};

	class Idle : public PlayerState {
	public:
		Idle() {};
		GLvoid Enter(Player* player, const Event& e = Event::None, const unsigned char& value = 0) override;
		GLvoid Exit() override;
		GLvoid Update() override;
		GLvoid HandleKeyDown(const unsigned char& key) override;
		GLvoid HandleKeyUp(const unsigned char& key) override;
	};

	class Walk : public PlayerState {
	public:
		Walk() {};
		GLvoid Enter(Player* player, const Event& e = Event::None, const unsigned char& value = 0) override;
		GLvoid Exit() override;
		GLvoid Update() override;
		GLvoid HandleKeyDown(const unsigned char& key) override;
		GLvoid HandleKeyUp(const unsigned char& key) override;
	};
}



class Player {
private:
	playerState::PlayerState* crntState = nullptr;
	GLchar dirFB = 0;
	GLchar dirLR = 0;

	glm::vec3 position = { 0, 0, 0 };
	glm::vec3 tpCameraPosition = { 0, 0, 0 };
	GLfloat tpCameraPitch = 0.0f;


	SharedObject* body = nullptr;
	Camera* fpCamera = nullptr;
	Camera* tpCamera = nullptr;

	Triangle* icon = nullptr;

	GLfloat velocity = 0.1f;

	GLfloat floor = 0.0f;
	GLfloat top = 0.0f;
	GLfloat yTop = 0.0f;
	GLfloat yBot = 0.0f;

public:
	Player(const glm::vec3& position);
	~Player();

	Cuboid* cuboid = nullptr;

	// state
	enum class State { Idle = 0, Walk, Run, Jump };
	GLvoid ChangeState(const State& playerState, const Event& e = Event::None, const unsigned char& value = 0);

	// Frame
	GLvoid Update();
	GLvoid Draw() const;
	GLvoid DrawIcon() const;

	// Process
	GLvoid ProcessKeyDown(const unsigned char& key);
	GLvoid ProcessKeyUp(const unsigned char& key);

	// Movement
	GLvoid Move();
	GLvoid Stop();
	GLvoid AddDirVert(const GLchar& direction);
	GLvoid AddDirHori(const GLchar& direction);
	inline constexpr GLchar GetDirVert() const
	{
		return dirFB;
	}
	inline constexpr GLchar GetDirHori() const
	{
		return dirLR;
	}
	GLvoid AddDir(const unsigned char& key);
	GLvoid SubDir(const unsigned char& key);

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