#include "stdafx.h"
#include "Player.h"
#include "Object.h"
#include "Camera.h"
#include "Timer.h"
#include "Map.h"
#include "Gun.h"
#include "Building.h"
#include "Sound.h"
#include "Turret.h"

#define STB_IMAGE_IMPLEMENTATION
#include <myGL/stb_image.h>
// extern
extern Map* crntMap;
extern BuildingManager* buildingManager;
extern SoundManager* soundManager;
extern TurretManager* turretManager;

using namespace playerState;

// movement key sets
const set<GLint> movFB = { 'w', 'W', 's', 'S' };
const set<GLint> movLR = { 'a', 'A', 'd', 'D' };
const set<GLint> movKeys = { 'w', 'W', 's', 'S', 'a', 'A', 's', 'S', 'd', 'D' };

////////////////////////////// [ State ] //////////////////////////////
/********** [ IDLE ] **********/
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
		else if (key == KEY_SPACEBAR)
		{
			soundManager->PlayEffectSound(EffectSound::Jump);
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

/********** [ WALK ] **********/
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
		else if (key == KEY_SPACEBAR)
		{
			mPlayer->ChangeState(Player::State::Jump);
		}
		else if (key == GLUT_KEY_SHIFT_L)
		{
			mPlayer->Run();
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
		else if (key == GLUT_KEY_SHIFT_L)
		{
			mPlayer->StopRun();
		}
		break;
	}
}


/********** [ JUMP ] **********/
GLvoid Jump::Enter(const Event& e, const GLint& value)
{
	t = 0;
	mPlayer->SetDir(KEY_SPACEBAR, UP);
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
			mPlayer->SetDir(KEY_SPACEBAR, DOWN);
		}
		else
		{
			if (isKeyUp)
			{
				mPlayer->SetDir(KEY_SPACEBAR, 0);
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
				mPlayer->SetDir(KEY_SPACEBAR, UP);
			}
		}
		return;
	}
	mPlayer->Move();
}
GLvoid Jump::HandleEvent(const Event& e, const GLint& key)
{
	if (key == KEY_SPACEBAR)
	{
		if (e == Event::KeyUp)
		{
			isKeyUp = GL_TRUE;
		}
		else if (e == Event::KeyDown)
		{
			isKeyUp = GL_FALSE;
			if (key == GLUT_KEY_SHIFT_L)
			{
				mPlayer->StopRun();
			}
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
	mObject = new SharedObject(GetIdentityModelObject(Models::Player));
	mObject->SetColor(WHITE);


	mFpCamera = new Camera();
	mFpCamera->SetPivot(&mPosition);
	mFpCamera->SetPosY(38);
	mFpCamera->SetFovY(110.0f);
	mFpCamera->SetLook(mObject->GetLook());

	mTpCamera = new Camera();
	mTpCamera->SetPivot(&mPosition);
	mTpCamera->SetPosY(100);
	mTpCamera->SetPosZ(-50);
	mTpCamera->SetFovY(110.0f);
	mTpCamera->Look(mObject->GetPosition());

	glm::vec3 gunPosition = glm::vec3(-PLAYER_RADIUS, mFpCamera->GetPviotedPosition().y - 20, 0);
	mGun = new Gun(gunPosition, &mPosition);

	mBoundingCircle = new Circle(mObject->GetRefPos(), PLAYER_RADIUS, { 0, 0.1f, 0 });
	mBoundingCircle->SetColor(BLUE);

	ChangeState(State::Idle);
}
Player::~Player()
{
	delete mObject;
	delete mFpCamera;
	delete mTpCamera;
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
	case KEY_SPACEBAR:
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

	mGun->Update();
}
GLvoid Player::Draw(const CameraMode& cameraMode) const
{
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
	GLfloat correction = 1.0f;
	if (mDirX != 0.0f && mDirZ != 0.0f)
	{
		correction = 0.8f;
	}

	glm::vec3 prevPos = mObject->GetPosition();
	if (mDirX != 0.0f) mObject->MoveX(mSpeed * mDirX * correction);
	if (mDirY != 0.0f) mObject->MoveY(mJumpSpeed * mDirY);
	if (mDirZ != 0.0f) mObject->MoveZ(mSpeed * mDirZ * correction);

	// xz collision
	if (crntMap->CheckCollision(mBoundingCircle) == GL_TRUE || buildingManager->CheckCollision(mBoundingCircle) == GL_TRUE)
	{
		mObject->SetPosX(prevPos.x);
		mObject->SetPosZ(prevPos.z);
	}


	static float frameTime = 0;
	if (frameTime > RUN_SOUND_TERM)
	{
		soundManager->PlayEffectSound(EffectSound::Run, 10.0f, GL_TRUE);
		frameTime = 0;
	}
	else frameTime += timer::DeltaTime();
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

	mFpCamera->SetLook(mObject->GetLook());
	mFpCamera->RotateLocal(mYaw, 0, 0);

	mTpCamera->SetLook(mObject->GetLook());
	mTpCamera->SetPosition({ 0, 100, -50 });
	mTpCamera->RotatePosition({ 0,0,0 }, Vector3::Up(), mPitch);
	mTpCamera->RotateLocal(mYaw -15.0f, 0, 0);

	mGun->Rotate(mYaw, mPitch);
	//gun->RotatePosition({ 0,0,0 }, Vector3::Up(), pitch);
}

glm::vec3 Player::GetPosition() const
{
	return mObject->GetPosition();
}

GLint Player::GetAmmo() const
{
	return mGun->GetAmmo();
}

GLint Player::GetMaxAmmo() const
{
	return mGun->GetMaxAmmo();
}

GLvoid Player::Damage(const GLfloat& damage)
{
	mHP -= damage;
	soundManager->PlayEffectSound(EffectSound::Hit);
	if (mHP <= 0)
	{
		//glutLeaveMainLoop();
	}
}

GLfloat Player::GetRadius() const
{
	return mObject->GetRadius();
}

GLfloat Player::GetHp() const
{
	return mHP;
}


GLvoid Player::Install_Turret()
{
	if (mHoldTurret > 0)
	{
		turretManager->Create(GetPosition());
		mHoldTurret--;
	}
}