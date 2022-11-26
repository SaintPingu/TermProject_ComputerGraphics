#include "stdafx.h"
#include "Player.h"
#include "Object.h"
#include "Camera.h"
#include "Timer.h"
#include "Map.h"
#include "Gun.h"


// extern
extern Map* crntMap;

using namespace playerState;

// movement key sets
const set<GLint> movFB = { 'w', 'W', 's', 'S' };
const set<GLint> movLR = { 'a', 'A', 'd', 'D' };
const set<GLint> movKeys = { 'w', 'W', 's', 'S', 'a', 'A', 's', 'S', 'd', 'D' };

////////////////////////////// [ State ] //////////////////////////////
// [ IDLE ] //
GLvoid Idle::Enter(const Event& e, const GLint& value)
{
	mPlayer->Stop();
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
			mPlayer->ChangeState(Player::State::Walk, e, key);
		}
		else if (key == GLUT_KEY_SPACEBAR)
		{
			mPlayer->ChangeState(Player::State::Jump);
		}
		break;
	case Event::KeyUp:
		if (movKeys.find(key) != movKeys.end())
		{
			mPlayer->ChangeState(Player::State::Walk, Event::KeyUp, key);
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
		mPlayer->AddDir(value);
		break;
	case Event::KeyUp:
		mPlayer->SubDir(value);
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
	mPlayer->Move();
}
GLvoid Walk::HandleEvent(const Event& e, const GLint& key)
{
	switch (e)
	{
	case Event::KeyDown:
		if (movKeys.find(key) != movKeys.end())
		{
			mPlayer->AddDir(key);
		}

		if (mPlayer->GetDirX() == 0 && mPlayer->GetDirZ() == 0)
		{
			mPlayer->ChangeState(Player::State::Idle, e, key);
		}
		else if (key == GLUT_KEY_SPACEBAR)
		{
			mPlayer->ChangeState(Player::State::Jump);
		}
		break;
	case Event::KeyUp:
		if (movKeys.find(key) != movKeys.end())
		{
			mPlayer->SubDir(key);
		}

		if (mPlayer->GetDirX() == 0 && mPlayer->GetDirZ() == 0)
		{
			mPlayer->ChangeState(Player::State::Idle, e, key);
		}
		break;
	}
}


// [ JUMP ] //
GLvoid Jump::Enter(const Event& e, const GLint& value)
{
	t = 0;
	mPlayer->SetDir(GLUT_KEY_SPACEBAR, UP);
}
GLvoid Jump::Exit()
{
}
GLvoid Jump::Update()
{
	t += timer::DeltaTime();
	if (t >= jumpTime)
	{
		if (mPlayer->GetDirY() == UP)
		{
			t = 0;
			mPlayer->SetDir(GLUT_KEY_SPACEBAR, DOWN);
		}
		else
		{
			if (isKeyUp)
			{
				mPlayer->SetDir(GLUT_KEY_SPACEBAR, 0);
				if (mPlayer->GetDirX() == 0 && mPlayer->GetDirZ() == 0)
				{
					mPlayer->ChangeState(Player::State::Idle);
				}
				else
				{
					mPlayer->ChangeState(Player::State::Walk);
				}
			}
			else
			{
				t = 0;
				mPlayer->SetDir(GLUT_KEY_SPACEBAR, UP);
			}
		}
		return;
	}
	mPlayer->Move();
}
GLvoid Jump::HandleEvent(const Event& e, const GLint& key)
{
	if (key == GLUT_KEY_SPACEBAR)
	{
		if (e == Event::KeyUp)
		{
			isKeyUp = true;
		}
		else if (e == Event::KeyDown)
		{
			isKeyUp = false;
		}
		return;
	}

	if (movKeys.find(key) != movKeys.end())
	{
		switch (e)
		{
		case Event::KeyDown:
			mPlayer->AddDir(key);
			break;
		case Event::KeyUp:
			mPlayer->SubDir(key);
			break;
		}
	}
}















////////////////////////////// [ Player ] //////////////////////////////
Player::Player(const glm::vec3& position, const CameraMode* cameraMode)
{
	mPosition = position;
	mTpCameraPosition = position;
	mCameraMode = cameraMode;
	mObject = new SharedObject(GetIdentityModelObject(IdentityObjects::Player));
	mObject->SetColor(WHITE);


	mFpCamera = new Camera();
	mFpCamera->SetPivot(&mPosition);
	mFpCamera->SetPosY(38);
	mFpCamera->SetFovY(110.0f);
	mFpCamera->SetLook(mObject->GetLook());

	glm::vec3 gunPosition = glm::vec3(-PLAYER_RADIUS, mFpCamera->GetPviotedPosition().y - 20, 0);
	mGun = new Gun(gunPosition, &mPosition);

	mBoundingCircle = new Circle(mObject->GetRefPos(), PLAYER_RADIUS, { 0, 1.0f, 0 });
	mBoundingCircle->SetColor(BLUE);

	ChangeState(State::Idle);
}
Player::~Player()
{
	delete mObject;
}



GLvoid Player::AddDir(const GLint& key)
{
	switch (key)
	{
	case 'w':
	case 'W':
		mDirZ += FRONT;
		break;
	case 's':
	case 'S':
		mDirZ += BACK;
		break;
	case 'a':
	case 'A':
		mDirX += LEFT;
		break;
	case 'd':
	case 'D':
		mDirX += RIGHT;
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
		mDirZ -= FRONT;
		break;
	case 's':
	case 'S':
		mDirZ -= BACK;
		break;
	case 'a':
	case 'A':
		mDirX -= LEFT;
		break;
	case 'd':
	case 'D':
		mDirX -= RIGHT;
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
		mDirY = value;
		break;
	}
}

GLvoid Player::ChangeState(const State& playerState, const Event& e, const GLint& value)
{
	if (mCrntState != nullptr)
	{
		mCrntState->Exit();
		delete mCrntState;
	}

	switch (playerState)
	{
	case State::Idle:
		mCrntState = new Idle(this);
		break;
	case State::Walk:
		mCrntState = new Walk(this);
		break;
	case State::Jump:
		mCrntState = new Jump(this);
		break;
	default:
		assert(0);
	}

	mCrntState->Enter(e, value);
}





GLvoid Player::Update()
{
	mCrntState->Update();

	mPosition = mObject->GetPviotedPosition();
	mTpCameraPosition = mObject->GetPviotedPosition();
	mTpCameraPosition.y += 0.5f;
	mTpCameraPosition.z -= 0.5f;
	RotatePosition(mTpCameraPosition, mObject->GetPviotedPosition(), mObject->GetUp(), mTpCameraPitch);

	mGun->Update();
}
GLvoid Player::Draw(const CameraMode& cameraMode) const
{
	mGun->Draw();
	if (cameraMode == CameraMode::FirstPerson)
	{
		return;
	}

	mObject->Draw();
	mBoundingCircle->Draw();
}
GLvoid Player::DrawIcon() const
{
}

GLvoid Player::ProcessKeyDown(const GLint& key)
{
	mCrntState->HandleEvent(Event::KeyDown, key);
}
GLvoid Player::ProcessKeyUp(const GLint& key)
{
	mCrntState->HandleEvent(Event::KeyUp, key);
}
GLvoid Player::ProcessMouse(GLint button, GLint state, GLint x, GLint y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			mGun->StartFire();
		}
		else if (state == GLUT_UP)
		{
			mGun->StopFire();
		}
		break;
	}
}

GLvoid Player::Move()
{
	glm::vec3 prevPos = mObject->GetPosition();
	if (mDirX != 0.0f) mObject->MoveX(mSpeed * mDirX);
	if (mDirY != 0.0f) mObject->MoveY(mJumpSpeed * mDirY);
	if (mDirZ != 0.0f) mObject->MoveZ(mSpeed * mDirZ);

	// xz collision
	if (crntMap->CheckCollision(mBoundingCircle) == GL_TRUE)
	{
		mObject->SetPosX(prevPos.x);
		mObject->SetPosZ(prevPos.z);
	}
}
GLvoid Player::Stop()
{
	mDirX = 0;
	mDirZ = 0;
}

GLvoid Player::Rotate(const GLfloat& yaw, const GLfloat& pitch, const GLfloat& roll)
{
	mYaw += yaw;
	mPitch += pitch;
	if (fabs(mYaw) > 89.0f)
	{
		mYaw = 89.0f * GetSign(mYaw);
	}

	mObject->RotateLocal(0, pitch, 0);

	mFpCamera->ResetLook();
	mFpCamera->SetLook(mObject->GetLook());
	mFpCamera->RotateLocal(mYaw, 0, 0);

	mGun->Rotate(mYaw, mPitch);
	//gun->RotatePosition({ 0,0,0 }, Vector3::Up(), pitch);
}

glm::vec3 Player::GetPosition() const
{
	return mObject->GetPosition();
}