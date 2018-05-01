// Host.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <KeyboardDrum.h>
#include <Windows.h>
#include <XMLDrumLoader.h>


using namespace std;

using namespace MapleDrum;
using namespace MapleDrum::Utilities;
using namespace tinyxml2;




int _tmain(int argc, _TCHAR* argv[])
{
	

	XMLDrumLoader xmlLoader;

	auto keyboardDrum = xmlLoader.CreateInsntanceFromFile("config.xml");
	if (!keyboardDrum)
		return -1;
	
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	
	auto instance = GetModuleHandle(NULL);
	keyboardDrum->Init(instance);
	keyboardDrum->Run();
	delete keyboardDrum;
	CoUninitialize();

	return 0;
}



