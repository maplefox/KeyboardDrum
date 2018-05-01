#include "Common.h"
#include "KeyboardInput.h"

namespace MapleDrum
{

	KeyboardInput::KeyboardInput()
	{


		ZeroMemory(keyboardState1, KeyCount);
		ZeroMemory(keyboardState2, KeyCount);
		mLastState = keyboardState1;
		mCurrentState = keyboardState2;

		


	}
	void KeyboardInput::Init(HINSTANCE instance, HWND window)
	{
		
		Release();
			//init keyboard device
		mInstance = instance;
		mWindow = window;
		hr = DirectInput8Create(mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDirectInput, NULL);
		hr = mDirectInput->CreateDevice(GUID_SysKeyboard, &mDevice, NULL);
		hr = mDevice->SetDataFormat(&c_dfDIKeyboard);
		hr = mDevice->SetCooperativeLevel(mWindow, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		hr = mDevice->Acquire();

		inited = true;
	}
	void KeyboardInput::Update()
	{
		//Swap chain
		mLastState = mCurrentState;
		if (mCurrentState == keyboardState1)
		{
			mCurrentState = keyboardState2;
		}
		else
		{
			mCurrentState = keyboardState1;
		}
		if (FAILED(mDevice->GetDeviceState(KeyCount, (LPVOID)mCurrentState)))
		{ // Try to reaqcuire the device
			if (SUCCEEDED(mDevice->Acquire()))
			{
				mDevice->GetDeviceState(KeyCount, (LPVOID)mCurrentState);
			}
		}
	}
	bool KeyboardInput::KeyDown(byte keyCode)
	{
		return (mCurrentState[keyCode] & 0x80) != 0;
	}
	bool KeyboardInput::KeyUp(byte keyCode)
	{
		return (mCurrentState[keyCode] & 0x80) == 0;
	}
	bool KeyboardInput::KeyPress(byte keyCode)
	{
		return ((mLastState[keyCode] & 0x80) == 0) && ((mCurrentState[keyCode] & 0x80) != 0);
	}
	bool KeyboardInput::KeyRelease(byte keyCode)
	{
		return ((mLastState[keyCode] & 0x80) != 0) && ((mCurrentState[keyCode] & 0x80) == 0);
	}
	void KeyboardInput::Release()
	{
		if (inited)
		{
			mDevice->Unacquire();
			mDevice->Release();
			mDirectInput->Release();
		}
		inited = false;
		
	}
	KeyboardInput::~KeyboardInput()
	{
		this->Release();
	}
}

