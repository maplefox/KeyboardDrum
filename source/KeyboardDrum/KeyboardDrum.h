#pragma once

#include "TypeDef.h"
#include "AudioDrum.h"
#include "VisualDrum.h"
class KeyboardDrum
{
public:
	
	
	KeyboardDrum(int windowWidth,int windowHeight,AudioDrum* audioDrum,VisualDrum* visualDrum);
	~KeyboardDrum();
	void Init(HINSTANCE instance);
	void Run();
	void Release();
private:
	bool mInited;
	int mWindowHeight;
	int mWindowWidth;
	wstring mWinClassName;
	HINSTANCE mInstance;                  //Ӧ��ʵ���ľ��
	HWND mWindow;

	AudioDrum* mAudioDrum;
	VisualDrum* mVisualDrum;
	void InitWindow();
};

