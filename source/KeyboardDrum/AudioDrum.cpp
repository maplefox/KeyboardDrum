#include "Common.h"
#include "AudioDrum.h"



using namespace MapleDrum;


inline int Min(int a, int b)
{
	return a < b ? a : b;
}

inline int Max(int a, int b)
{
	return a > b ? a : b;
}


WAVEFORMATEX AudioDrum::DefaultFormat =
{
	WAVE_FORMAT_IEEE_FLOAT,
	2,                       //channel
	44100,                   //samPerSec
	352800,                  //bytePerSec
	8,                       //blockAlign
	32,                      //bitPerSam
	0                        //cbsize
};



AudioDrum::AudioDrum(DrumInput* input,DrumProcessor* processor,AudioEngine* audioEngine)
	:mTimeCounter(), mInited(false), mRunning(false), mFormat(DefaultFormat),
	mInput(input), mDrumProcessor(processor),mAudioEngine(audioEngine)
{

}

AudioDrum::~AudioDrum()
{
	delete mInput;
	delete mDrumProcessor;
	delete mAudioEngine;
	
}

void AudioDrum::Init(HINSTANCE instance,HWND window , const EventsPtr* eventOutput)
{
	mPlayEvents = eventOutput;
	mKeyEvents = mInput->EventsOutput();
	auto mixBuffer = mDrumProcessor->AudioOutput();
	mInput->Init(instance, window);	
	mDrumProcessor->Init(mKeyEvents);
	mAudioEngine->Init((byte*)mixBuffer);
	mInited = true;
}
void AudioDrum::Run()
{
	
	running = true;
	stopSignal = false;
	runThread = thread(&AudioDrum::MainExecute, this);
}



void AudioDrum::Stop()
{
	if (!running)
		return;
	stopSignal = true;
	runThread.join();
	mRunning = false;
}

void AudioDrum::MainExecute()
{
	
	//Reset
	mDrumProcessor->StopAll();

	mAudioEngine->Run();
	mTimeCounter.Reset();
	
	while (true)
	{
		float lapsedTime = mTimeCounter.Update();
		//get key state
		mInput->Update(lapsedTime);	
		OutputEvents();
		mDrumProcessor->ComputData();	
		mAudioEngine->SubmitBuffer();
		if(stopSignal)
			break;
		
		///printf("\r%f  ", lapsedTime);
	}

	mAudioEngine->Stop();
}




void AudioDrum::OutputEvents()
{
	auto& keyEvents = *mKeyEvents;

	int evtNum = keyEvents.size();
	for (int i = 0; i<evtNum; i++)
	{
		auto& playEvents = **mPlayEvents;
		auto& keyEvt = keyEvents[i];
		auto& playEvt = playEvents[i];

		if (keyEvt.type && keyEvt.value > playEvt.value)
		{
			playEvt.type += keyEvt.type;
			playEvt.value = keyEvt.value;
		}
	}
}


void  AudioDrum::Release()
{
	
	

}
