#pragma once

#include "TypeDef.h"

namespace MapleDrum
{
	
	class KeyboardInput
	{
	public:
		
		KeyboardInput();
		~KeyboardInput();
		void Init(HINSTANCE instance, HWND window);
		
		void Update();
		void Release();
		bool KeyDown(byte keyCode);
		bool KeyUp(byte keyCode);
		bool KeyPress(byte keyCode);
		bool KeyRelease(byte keyCode);

	private:
		bool inited = false;
		HRESULT hr;
		HINSTANCE mInstance = NULL;                  //应用实例的句柄
		HWND mWindow = NULL;

		LPDIRECTINPUT8 mDirectInput = NULL;          //DirectInput Object
		LPDIRECTINPUTDEVICE8 mDevice = NULL;
		
		static const int KeyCount = 256;
		byte* mCurrentState;
		byte* mLastState;
		byte keyboardState1[KeyCount],keyboardState2[KeyCount];
		

	};
}


