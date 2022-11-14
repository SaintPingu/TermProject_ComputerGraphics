#include "stdafx.h"
#include "Player.h"
#include "Object.h"
#include "Camera.h"
#include "Timer.h"

using namespace playerState;

// movement key sets
const set<GLint> movFB = { 'w', 'W', 's', 'S' };
const set<GLint> movLR = { 'a', 'A', 'd', 'D' };
const set<GLint> movKeys = { 'w', 'W', 's', 'S', 'a', 'A', 's', 'S', 'd', 'D' };

////////////////////////////// [ State ] //////////////////////////////
// [ IDLE ] //
GLvoid Idle::Enter(const Event& e, const GLint& value)
{
	player->Stop();
}
GLvoid Idle::Exit()
{
}
GLvoid Idle::Update()
{
	
}

GLvoid Idle::HandleEvent(const Event& e, const GLint& key)
{
	switch (e)
	{
	case Event::KeyDown:
		if(movKeys.find(key) != movKeys.end())
		{
			player->ChangeState(Player::State::Walk, e, key);
		}
		else if (key == GLUT_KEY_SPACEBAR)
		{
			player->ChangeState(Player::State::Jump);
		}
		break;
	case Event::KeyUp:
		if (movKeys.find(key) != movKeys.end())
		{
			player->ChangeState(Player::State::Walk, Event::KeyUp, key);
		}
		break;
	}
}

// [ WALK ] //
GLvoid Walk::Enter(const Event& e, const GLint& value)
{
	switch (e)
	{
	case Event::KeyDown:
		player->AddDir(value);
		break;
	case Event::KeyUp:
		player->SubDir(value);
		break;
	default:
		break;
	}
}
GLvoid Walk::Exit()
{
}
GLvoid Walk::Update()
{
	player->Move();
}
GLvoid Walk::HandleEvent(const Event& e, const GLint& key)
{
	switch (e)
	{
	case Event::KeyDown:
		if (movKeys.find(key) != movKeys.end())
		{
			player->AddDir(key);
		}

		if (player->GetDirX() == 0 && player->GetDirZ() == 0)
		{
			player->ChangeState(Player::State::Idle, e, key);
		}
		else if (key == GLUT_KEY_SPACEBAR)
		{
			player->ChangeState(Player::State::Jump);
		}
		break;
	case Event::KeyUp:
		if (movKeys.find(key) != movKeys.end())
		{
			player->SubDir(key);
		}

		if (player->GetDirX() == 0 && player->GetDirZ() == 0)
		{
			player->ChangeState(Player::State::Idle, e, key);
		}
		break;
	}
}


// [ JUMP ] //
GLvoid Jump::Enter(const Event& e, const GLint& value)
{
	t = 0;
	player->SetDir(GLUT_KEY_SPACEBAR, UP);
}
GLvoid Jump::Exit()
{
}
GLvoid Jump::Update()
{
	t += timer::DeltaTime();
	if (t >= jumpTime)
	{
		if (player->GetDirY() == UP)
		{
			t = 0;
			player->SetDir(GLUT_KEY_SPACEBAR, DOWN);
		}
		else
		{
			player->SetDir(GLUT_KEY_SPACEBAR, 0);
			if (player->GetDirX() == 0 && player->GetDirZ() == 0)
			{
				player->ChangeState(Player::State::Idle);
			}
			else
			{
				player->ChangeState(Player::State::Walk);
			}
		}
		return;
	}
	player->Move();
}
GLvoid Jump::HandleEvent(const Event& e, const GLint& key)
{
	if (key == GLUT_KEY_SPACEBAR)
	{
		return;
	}

	if (movKeys.find(key) != movKeys.end())
	{
		switch (e)
		{
		case Event::KeyDown:
			player->AddDir(key);
			break;
		case Event::KeyUp:
			player->SubDir(key);
			break;
		}
	}
}


////////////////////////////// [ Player ] //////////////////////////////
GLvoid Player::AddDir(const GLint& key)
{
	switch (key)
	{
	case 'w':
	case 'W':
		dirZ += FRONT;
		break;
	case 's':
	case 'S':
		dirZ += BACK;
		break;
	case 'a':
	case 'A':
		dirX += LEFT;
		break;
	case 'd':
	case 'D':
		dirX += RIGHT;
		break;
	default:
		break;
	}
}
GLvoid Player::SubDir(const GLint& key)
{
	switch (key)
	{
	case 'w':
	case 'W':
		dirZ -= FRONT;
		break;
	case 's':
	case 'S':
		dirZ -= BACK;
		break;
	case 'a':
	case 'A':
		dirX -= LEFT;
		break;
	case 'd':
	case 'D':
		dirX -= RIGHT;
		break;
	default:
		break;
	}
}
GLvoid Player::SetDir(const GLint& key, const GLint& value)
{
	switch (key)
	{
	case GLUT_KEY_SPACEBAR:
		dirY = value;
		break;
	}
}

GLvoid Player::ChangeState(const State& playerState, const Event& e, const GLint& value)
{
	if (crntState != nullptr)
	{
		crntState->Exit();
		delete crntState;
	}

	switch (playerState)
	{
	case State::Idle:
		crntState = new Idle(this);
		break;
	case State::Walk:
		crntState = new Walk(this);
		break;
	case State::Jump:
		crntState = new Jump(this);
		break;
	default:
		assert(0);
	}

	crntState->Enter(e, value);
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

GLvoid Player::ProcessKeyDown(const GLint& key)
{
	// Bug : key value changed when the key with a control key
	printf("%d\n", key);
	crntState->HandleEvent(Event::KeyDown, key);
}
GLvoid Player::ProcessKeyUp(const GLint& key)
{
	crntState->HandleEvent(Event::KeyUp, key);
}

GLvoid Player::Move()
{
	body->MoveX(speed * dirX);
	body->MoveY(jumpSpeed * dirY);
	body->MoveZ(speed * dirZ);
}
GLvoid Player::Stop()
{
	dirX = 0;
	dirZ = 0;
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




