#pragma once

#include "TypeDef.h"
#include "AudioEngine.h"
#include "DrumInput.h"
#include "MapleTime.h"
#include "DrumProcessor.h"


namespace MapleDrum
{
	
	

	class AudioDrum
	{
		
	public:
		
		
		static WAVEFORMATEX DefaultFormat;
		AudioDrum(DrumInput* input, DrumProcessor* processor, AudioEngine* audioEngine);
		~AudioDrum();
		void Init(HINSTANCE instance, HWND window, const EventsPtr* eventOutput);
		void Run();
		void Stop();
		void Release();
		
	private:
		
		volatile bool stopSignal = false;
		volatile bool running = false;
		bool mInited;
		bool mRunning;
		thread runThread;

		HRESULT hr;

		MapleTime mTimeCounter;
		WAVEFORMATEX mFormat;
		
		AudioEngine* mAudioEngine;
		DrumInput* mInput;
		DrumProcessor* mDrumProcessor;
		const EventsPtr* mPlayEvents;   //output
		const vector<EventInfo>* mKeyEvents;

		void MainExecute();
		
		void OutputEvents();
		
		
	};

}


