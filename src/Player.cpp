#include "stdafx.h"
#include "Player.h"
#include "Object.h"
#include "Camera.h"

using namespace playerState;

// movement key sets
const set<unsigned char> movVert = { 'w', 'W', 's', 'S' };
const set<unsigned char> movHori = { 'a', 'A', 'd', 'D' };
const set<unsigned char> movKeys = { 'w', 'W', 's', 'S', 'a', 'A', 's', 'S', 'd', 'D' };

GLvoid Idle::Enter(Player* player, const unsigned char& e)
{
	PlayerState::Enter(player);

	printf("Enter Idle\n");
	player->Stop();
}
GLvoid Idle::Exit()
{
	PlayerState::Exit();
	printf("Exit Idle\n");
}
GLvoid Idle::Update()
{
	
}
GLvoid Idle::HandleKeyDown(const unsigned char& key)
{
	if(movKeys.find(key) != movKeys.end())
	{
		player->ChangeState(Player::State::Walk, key);
	}
}
GLvoid Idle::HandleKeyUp(const unsigned char& key)
{
	if (movVert.find(key) != movVert.end())
	{
		player->ChangeState(Player::State::Walk, key);
	}
}


GLvoid Walk::Enter(Player* player, const unsigned char& e)
{
	PlayerState::Enter(player);
	player->AddDir(e);

	printf("Enter Walk\n");
}
GLvoid Walk::Exit()
{
	PlayerState::Exit();
	printf("Exit Walk\n");
}
GLvoid Walk::Update()
{
	player->Move();
}
GLvoid Walk::HandleKeyDown(const unsigned char& key)
{
	player->AddDir(key);
	if (player->GetDirHori() == 0 && player->GetDirVert() == 0)
	{
		player->ChangeState(Player::State::Idle, key);
	}
}
GLvoid Walk::HandleKeyUp(const unsigned char& key)
{
	player->SubDir(key);
	if (player->GetDirHori() == 0 && player->GetDirVert() == 0)
	{
		player->ChangeState(Player::State::Idle, key);;
	}
}
GLvoid Player::AddDir(const unsigned char& key)
{
	switch (key)
	{
	case 'w':
	case 'W':
		dirVert += FRONT;
		break;
	case 's':
	case 'S':
		dirVert += BACK;
		break;
	case 'a':
	case 'A':
		dirHori += LEFT;
		break;
	case 'd':
	case 'D':
		dirHori += RIGHT;
		break;
	}
}
GLvoid Player::SubDir(const unsigned char& key)
{
	switch (key)
	{
	case 'w':
	case 'W':
		dirVert -= FRONT;
		break;
	case 's':
	case 'S':
		dirVert -= BACK;
		break;
	case 'a':
	case 'A':
		dirHori -= LEFT;
		break;
	case 'd':
	case 'D':
		dirHori -= RIGHT;
		break;
	}
}

GLvoid Player::ChangeState(const State& playerState, const unsigned char& e)
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

	crntState->Enter(this, e);
}



Player::Player(const glm::vec3& position)
{
	this->position = position;
	this->tpCameraPosition = position;
	body = new SharedObject(GetIdentityPlayer());

	fpCamera = new Camera();
	fpCamera->SetPivot(&this->position);
	fpCamera->MoveY(0.2f);
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
	body->MoveZ(velocity * dirVert);
	body->MoveX(velocity * dirHori);
}
GLvoid Player::Stop()
{
	dirHori = 0;
	dirVert = 0;
}
GLvoid Player::AddDirVert(const GLchar& direction)
{
	dirVert += direction;
}
GLvoid Player::AddDirHori(const GLchar& direction)
{
	dirHori += direction;
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




