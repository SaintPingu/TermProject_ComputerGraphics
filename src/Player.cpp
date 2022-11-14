#include "stdafx.h"
#include "Player.h"
#include "Object.h"
#include "Camera.h"

using namespace playerState;

// movement key sets
const set<unsigned char> movFB = { 'w', 'W', 's', 'S' };
const set<unsigned char> movLR = { 'a', 'A', 'd', 'D' };
const set<unsigned char> movKeys = { 'w', 'W', 's', 'S', 'a', 'A', 's', 'S', 'd', 'D' };

GLvoid Idle::Enter(Player* player, const Event& e, const unsigned char& value)
{
	PlayerState::Enter(player);

	player->Stop();
}
GLvoid Idle::Exit()
{
	PlayerState::Exit();
}
GLvoid Idle::Update()
{
	
}
GLvoid Idle::HandleKeyDown(const unsigned char& key)
{
	if(movKeys.find(key) != movKeys.end())
	{
		player->ChangeState(Player::State::Walk, Event::KeyDown, key);
	}
}
GLvoid Idle::HandleKeyUp(const unsigned char& key)
{
	if (movKeys.find(key) != movKeys.end())
	{
		player->ChangeState(Player::State::Walk, Event::KeyUp, key);
	}
}


GLvoid Walk::Enter(Player* player, const Event& e, const unsigned char& value)
{
	PlayerState::Enter(player);
	if (e == Event::KeyDown)
	{
		player->AddDir(value);
	}
	else
	{
		player->SubDir(value);
	}
}
GLvoid Walk::Exit()
{
	PlayerState::Exit();
}
GLvoid Walk::Update()
{
	player->Move();
}
GLvoid Walk::HandleKeyDown(const unsigned char& key)
{
	const Event e = Event::KeyDown;

	player->AddDir(key);
	if (player->GetDirHori() == 0 && player->GetDirVert() == 0)
	{
		player->ChangeState(Player::State::Idle, e, key);
	}
}
GLvoid Walk::HandleKeyUp(const unsigned char& key)
{
	const Event e = Event::KeyUp;

	player->SubDir(key);
	if (player->GetDirHori() == 0 && player->GetDirVert() == 0)
	{
		player->ChangeState(Player::State::Idle, e, key);
	}
}
GLvoid Player::AddDir(const unsigned char& key)
{
	switch (key)
	{
	case 'w':
	case 'W':
		dirFB += FRONT;
		break;
	case 's':
	case 'S':
		dirFB += BACK;
		break;
	case 'a':
	case 'A':
		dirLR += LEFT;
		break;
	case 'd':
	case 'D':
		dirLR += RIGHT;
		break;
	}
}
GLvoid Player::SubDir(const unsigned char& key)
{
	switch (key)
	{
	case 'w':
	case 'W':
		dirFB -= FRONT;
		break;
	case 's':
	case 'S':
		dirFB -= BACK;
		break;
	case 'a':
	case 'A':
		dirLR -= LEFT;
		break;
	case 'd':
	case 'D':
		dirLR -= RIGHT;
		break;
	}
}

GLvoid Player::ChangeState(const State& playerState, const Event& e, const unsigned char& value)
{
	if (crntState != nullptr)
	{
		crntState->Exit();
		delete crntState;
	}

	switch (playerState)
	{
	case State::Idle:
		crntState = new Idle();
		break;
	case State::Walk:
		crntState = new Walk();
		break;
	default:
		assert(0);
	}

	crntState->Enter(this, e, value);
}



Player::Player(const glm::vec3& position)
{
	this->position = position;
	this->tpCameraPosition = position;
	body = new SharedObject(GetIdentityPlayer());

	fpCamera = new Camera();
	fpCamera->SetPivot(&this->position);
	fpCamera->MoveY(0.7f);
	fpCamera->SetFovY(110.0f);
	fpCamera->SetLook(body->GetLook());

	ChangeState(State::Idle);
}
Player::~Player()
{
	delete body;
}

GLvoid Player::Update()
{
	crntState->Update();

	position = body->GetPosition();
	tpCameraPosition = body->GetPosition();
	tpCameraPosition.y += 0.5f;
	tpCameraPosition.z -= 0.5f;
	RotatePosition(tpCameraPosition, body->GetPosition(), body->GetUp(), tpCameraPitch);
}
GLvoid Player::Draw() const
{
	body->Draw();
}
GLvoid Player::DrawIcon() const
{
}

GLvoid Player::ProcessKeyDown(const unsigned char& key)
{
	crntState->HandleKeyDown(key);
}
GLvoid Player::ProcessKeyUp(const unsigned char& key)
{
	crntState->HandleKeyUp(key);
}

GLvoid Player::Move()
{
	body->MoveZ(velocity * dirFB);
	body->MoveX(velocity * dirLR);
}
GLvoid Player::Stop()
{
	dirLR = 0;
	dirFB = 0;
}
GLvoid Player::AddDirVert(const GLchar& direction)
{
	dirFB += direction;
}
GLvoid Player::AddDirHori(const GLchar& direction)
{
	dirLR += direction;
}

GLvoid Player::Rotate(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll)
{
	body->RotateLocal(yaw, pitch, roll);
	fpCamera->SetLook(body->GetLook());
	/*tpCamera->Look(body->GetPosition());
	tpCameraPitch += pitch;*/

	//icon->RotateLocal(Vector3::Up(), pitch);
}

glm::vec3 Player::GetPosition() const
{
	return body->GetPosition();
}




