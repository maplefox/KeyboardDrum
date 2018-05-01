#pragma once

#include <Windows.h>
#include <string>
using namespace std;
namespace MapleDrum
{
	namespace Utilities
	{

		struct RawAudio
		{
			WAVEFORMATEX format;
			int size;
			byte* data;
		};
		bool LoadRawAudio(string path, RawAudio& audio);
		
	}
}


