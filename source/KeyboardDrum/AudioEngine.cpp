#include "Common.h"
#include "AudioEngine.h"

namespace MapleDrum
{
	
	
	
	AudioEngine::AudioEngine(int bufferNum, int blockNum,const WAVEFORMATEX& format) 
		:curFreeIdx(0),mBufferNum(bufferNum), mBufferSize(blockNum*format.nBlockAlign),mInited(false),
		waveFormat(format), callBack(mBufferNum),inputBuffer(nullptr)
	{
		
		bufferData = new byte[mBufferNum*mBufferSize];
		
		
	}

	void AudioEngine::Init(const byte* inputBuffer)
	{
		Release();
		this->inputBuffer = inputBuffer;
		hr = XAudio2Create(&mXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
		hr = mXAudio2->CreateMasteringVoice(&mMasterVoice);
		hr = mXAudio2->CreateSourceVoice(&mSourceVoice, &waveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &callBack);
		mInited = true;

	}
	byte* AudioEngine::CurrentBufferPtr()
	{	
		return  &bufferData[curFreeIdx*mBufferSize];
	}
	
	int AudioEngine::GetBufferSize() const
	{
		return mBufferSize;
	}
	void AudioEngine::SubmitBuffer()
	{
			 	
		WaitForSingleObject(callBack.hBufferSemaphore, INFINITE);
		//callBack.resNum--;
		//printf("resNum: %d ", callBack.resNum);
		byte* submitData = CurrentBufferPtr();
		CopyMemory(submitData, inputBuffer, mBufferSize);
		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = mBufferSize;
		buffer.pAudioData = submitData;
		mSourceVoice->SubmitSourceBuffer(&buffer);
		
		curFreeIdx++;
		curFreeIdx %= mBufferNum;
		
		
	}
	void AudioEngine::Run()
	{
		
		hr = mSourceVoice->Start();
	}
	void AudioEngine::Stop()
	{
		
		mSourceVoice->Stop();
		mSourceVoice->FlushSourceBuffers();
		
	}

	void AudioEngine::Release()
	{
		if (mInited)
		{
			mSourceVoice->DestroyVoice();
			mMasterVoice->DestroyVoice();
			mXAudio2->Release();
		}
		mInited = false;
		
	}

	AudioEngine::~AudioEngine()
	{
		Release();
		delete[] bufferData;
	}

}
