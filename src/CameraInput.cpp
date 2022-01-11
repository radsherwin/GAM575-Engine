#include "CameraInput.h"
#include "GameObjectManager.h"
#include "AnimationManager.h" //For demo

using namespace Azul;

CameraInput* CameraInput::posInstance = nullptr;

CameraInput::CameraInput(GLFWwindow* _window)
	:pWindow(_window)
{
	this->obj.set(0.0f, 0.0f, 0.0f);
}

void CameraInput::SwitchCamera(Keyboard* pKey)
{
	
}

KeyPress walkKey(Keyboard::KEY_1);
KeyPress dieLeftKey(Keyboard::KEY_2);
KeyPress shotDownKey(Keyboard::KEY_3);
KeyPress hitRightKey(Keyboard::KEY_4);
KeyPress runKey(Keyboard::KEY_5);

void CameraInput::PlayDemo(Keyboard* pKey)
{
	//------------
	//	Walk
	//------------
	if (walkKey)
	{
		Trace::out("Walking!");
		AnimationManager::SetClip(Clip::Name::WALK, AnimController::AnimName::MESH1);
	}

	//------------
	//	Die_Left
	//------------
	if (dieLeftKey)
	{
		AnimationManager::SetClip(Clip::Name::DIE_LEFT, AnimController::AnimName::MESH1);
	}

	//------------
	//	Shot_Down
	//------------
	if (shotDownKey)
	{
		AnimationManager::SetClip(Clip::Name::SHOT_DOWN, AnimController::AnimName::MESH1);
	}

	//------------
	//	Hit_Right
	//------------
	if (hitRightKey)
	{
		AnimationManager::SetClip(Clip::Name::HIT_RIGHT, AnimController::AnimName::MESH1);
	}

	//------------
	//	Run
	//------------
	if (runKey)
	{
		AnimationManager::SetClip(Clip::Name::RUN, AnimController::AnimName::MESH1);

	}

}

KeyPress speedUPKey(Keyboard::KEY_EQUAL);
KeyPress slowDownKey(Keyboard::KEY_MINUS);
KeyPress revKey(Keyboard::KEY_Q);
KeyPress fwdKey(Keyboard::KEY_W);
KeyPress ppKey(Keyboard::KEY_SPACE);

void CameraInput::DemoChangeSpeed(Keyboard* pKey)
{

	if(speedUPKey)
	{
		AnimationManager::IncreaseSpeed(1.25f);
	}

	if(slowDownKey)
	{
		AnimationManager::DecreaseSpeed(1.25f);
	}

	if(revKey)
	{
		AnimationManager::Reverse();
	}

	if(fwdKey)
	{
		AnimationManager::Forward();
	}

	if(ppKey)
	{
		AnimationManager::PlayPause();
	}

	//Demo 2
	static bool keyState6 = false;
	if (pKey->GetKeyState(Keyboard::KEY_E) && !keyState6)
	{
		keyState6 = true;
	}

	if (pKey->GetKeyReleaseState(Keyboard::KEY_E) && keyState6)
	{
		AnimationManager::Demo2();
		keyState6 = false;
	}
	
}

void CameraInput::Create(GLFWwindow* _window)
{
    if (posInstance == nullptr)
    {
        posInstance = new CameraInput(_window);
    }
}

void CameraInput::Destroy()
{
    CameraInput* pMan = CameraInput::privGetInstance();
    assert(pMan != nullptr);
	AZUL_UNUSED_VAR(pMan);

    delete CameraInput::posInstance;
    CameraInput::posInstance = nullptr;
}

CameraInput* CameraInput::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}



void CameraInput::Update()
{
	CameraInput* pMan = CameraInput::privGetInstance();
    Camera* pCam = CameraManager::GetCurrent(Camera::Type::PERSPECTIVE_3D);
	Keyboard* pKey = InputManager::GetKeyboard();
	

	pMan->PlayDemo(pKey); //Demo of cycling through animation
	pMan->DemoChangeSpeed(pKey);

	pMan->SwitchCamera(pKey);

	//Reset
	if (pKey->GetKeyState(Keyboard::KEY_Z))
	{
		// Reset	
		pMan->obj.set(0, 0, 0);
		pCam->GetOriginal();
	}
	//Zoom in
	if (pKey->GetKeyState(Keyboard::KEY_I) && !pCam->bStatic)
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		//	tar = obj;

		pos = pos + 0.03f * forwardNorm;
		//tar = tar + 0.03f * forwardNorm;
		up = up + 0.03f * forwardNorm;
		if((pos - tar).mag() > 0.5f)
		{
			pCam->SetHelper(up, tar, pos);
		}
		
	}
	//Zoom out
	if (pKey->GetKeyState(Keyboard::KEY_O) && !pCam->bStatic)
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		tar = pMan->obj;

		pos = pos - 0.03f * forwardNorm;
		up = up - 0.03f * forwardNorm;

		pCam->SetHelper(up, tar, pos);
	}


	//TURN RIGHT
	if (pKey->GetKeyState(Keyboard::KEY_ARROW_RIGHT) && !pCam->bStatic)
	{
		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		tar = pMan->obj;
		Matrix Trans(Matrix::Trans::XYZ, tar);
		Matrix NegTrans(Matrix::Trans::XYZ, -tar);

		Matrix Rot;
		Rot.set(Matrix::Rot::AxisAngle, upNorm, +0.03f);

		Matrix M = NegTrans * Rot * Trans;

		//M = Rot;

		up = up * M;
		pos = pos * M;
		tar = tar * M;

		pCam->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_LEFT) && !pCam->bStatic)
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm

		tar = pMan->obj;
		Matrix Trans(Matrix::Trans::XYZ, tar);
		Matrix NegTrans(Matrix::Trans::XYZ, -tar);

		Matrix Rot;
		Rot.set(Matrix::Rot::AxisAngle, upNorm, -0.03f);

		Matrix M = NegTrans * Rot * Trans;

		up = up * M;
		pos = pos * M;
		tar = tar * M;

		pCam->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_DOWN) && !pCam->bStatic)
	{
		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		tar = pMan->obj;

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm

		Matrix Trans(Matrix::Trans::XYZ, tar);
		Matrix NegTrans(Matrix::Trans::XYZ, -tar);
		Matrix Rot;
		Rot.set(Matrix::Rot::AxisAngle, rightNorm, 0.03f);

		Matrix M = NegTrans * Rot * Trans;

		up = up * M;
		pos = pos * M;
		tar = tar * M;

		pCam->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_UP) && !pCam->bStatic)
	{
		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		tar = pMan->obj;

		Matrix Trans(Matrix::Trans::XYZ, tar);
		Matrix NegTrans(Matrix::Trans::XYZ, -tar);
		Matrix Rot;
		Rot.set(Matrix::Rot::AxisAngle, rightNorm, -0.03f);

		Matrix M = NegTrans * Rot * Trans;

		up = up * M;
		pos = pos * M;
		tar = tar * M;

		pCam->SetHelper(up, tar, pos);
	}
}
