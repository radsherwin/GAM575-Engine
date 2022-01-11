#ifndef CAMERA_INPUT_H
#define CAMERA_INPUT_H

#include "Camera.h"
#include "CameraManager.h"
#include "Keyboard.h"
#include "InputManager.h"

namespace Azul
{
struct KeyPress
{
	bool pressed = false;
	Keyboard::Key key;

	KeyPress(Keyboard::Key key)
	{
		this->key = key;
	}
	operator bool()
	{
		Keyboard *pKey = InputManager::GetKeyboard();
		if (pKey->GetKeyState(key))
		{
			pressed = true;
		}

		if (pKey->GetKeyReleaseState(key) && pressed)
		{
			pressed = false;
			return true;
		}
		return false;
	}
};

class CameraInput : public Align16
{
public:
	CameraInput(GLFWwindow *_window);
	CameraInput() = delete;
	CameraInput(const CameraInput &) = delete;
	CameraInput &operator=(const CameraInput &) = delete;
	~CameraInput() = default;

	void SwitchCamera(Keyboard *pKey);
	void PlayDemo(Keyboard *pKey);
	void DemoChangeSpeed(Keyboard *pKey);

	static void Create(GLFWwindow *_window);
	static void Destroy();
	static void Update();

private:
	static CameraInput *privGetInstance();

private:
	Vect obj;
	GLFWwindow *pWindow; //Temporary, not sure if I want CameraInput to point to GLFWwindow currently, but it's easy to read for demo
	static CameraInput *posInstance;
	int mode = 1;
};
}

#endif