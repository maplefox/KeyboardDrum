#include "Common.h"
#include "KeyboardDrum.h"



KeyboardDrum::KeyboardDrum(int windowWidth, int windowHeight,
	AudioDrum* audioDrum, VisualDrum* visualDrum):
	mInited(false), mWindowWidth(windowWidth),mWindowHeight(windowHeight),mWindow(nullptr),
	mAudioDrum(audioDrum),mVisualDrum(visualDrum)
{
}


KeyboardDrum::~KeyboardDrum()
{
	delete mAudioDrum;
	delete mVisualDrum;
	UnregisterClass(mWinClassName.c_str(), mInstance);
}


void KeyboardDrum::Init(HINSTANCE instance)
{
	mInstance = instance;
	InitWindow();
	auto graphicsEvts = mVisualDrum->EventInput();
	mAudioDrum->Init(instance,mWindow,graphicsEvts);
	mVisualDrum->Init(mInstance, mWindow);
}

POINT CenterWindow(int windowWidth, int windowHeight)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	POINT center;
	center.x = (screenWidth - windowWidth) / 2;
	center.y = (screenHeight - windowHeight) / 2;

	return center;
}

LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(windowHandle, message, wParam, lParam);
}


void KeyboardDrum::InitWindow()
{
	mWinClassName = L"VisualDrumClass";

	WNDCLASSEX mWindowClass;

	ZeroMemory(&mWindowClass, sizeof(mWindowClass));
	mWindowClass.cbSize = sizeof(WNDCLASSEX);
	mWindowClass.style = CS_CLASSDC;
	mWindowClass.lpfnWndProc = WndProc;
	mWindowClass.hInstance = mInstance;
	mWindowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	mWindowClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	mWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	mWindowClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	mWindowClass.lpszClassName = mWinClassName.c_str();

	RECT windowRectangle = { 0, 0, mWindowWidth, mWindowHeight };
	AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

	RegisterClassEx(&mWindowClass);
	POINT center = CenterWindow(mWindowWidth, mWindowHeight);
	mWindow = CreateWindow(mWinClassName.c_str(), L"AudioDrum", WS_OVERLAPPEDWINDOW, center.x, center.y, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, mInstance, nullptr);

	ShowWindow(mWindow, SW_SHOW);
	UpdateWindow(mWindow);
}


void  KeyboardDrum::Run()
{
	
	mAudioDrum->Run();
	mVisualDrum->Run();
	MSG message;
	ZeroMemory(&message, sizeof(message));
	while (GetMessage(&message, nullptr, 0, 0))
	{	
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	
	mVisualDrum->Stop();
	mAudioDrum->Stop();
	/*
	while (message.message != WM_QUIT)
	{
	if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
	{
	TranslateMessage(&message);
	DispatchMessage(&message);
	}else{}*/
	
}
