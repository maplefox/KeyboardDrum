#pragma once

#include "TypeDef.h"
#include "MapleTime.h"

namespace MapleDrum
{


	class VoiceCallback : public IXAudio2VoiceCallback
	{
	public:
		HANDLE hBufferSemaphore;
		int resNum;
		VoiceCallback(int bufferNum) :
			resNum(bufferNum),
			hBufferSemaphore(CreateSemaphore(NULL, bufferNum, bufferNum, NULL))
		{}
		//                                                        或许需要通过设置自旋锁来提高效率
		VoiceCallback() :VoiceCallback(2){}
		~VoiceCallback(){ CloseHandle(hBufferSemaphore); }

		void _stdcall OnBufferEnd(void*)
		{
			resNum++;
			ReleaseSemaphore(hBufferSemaphore, 1, NULL);
		}
		//Unused methods are stubs
		void _stdcall OnVoiceProcessingPassEnd() { }
		void _stdcall OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
		void _stdcall OnStreamEnd() { }
		void _stdcall OnBufferStart(void * pBufferContext) {   }
		void _stdcall OnLoopEnd(void * pBufferContext) {    }
		void _stdcall OnVoiceError(void * pBufferContext, HRESULT Error) { }
		
	};
	
	//负责流式播放音频的底层引擎
	//typedef unsigned char byte;
	class AudioEngine
	{
	public:
		AudioEngine(int bufferNum, int bufferSize,const WAVEFORMATEX& format);

		void Init(const byte* inputBuffer);
		void Release();

		//向音频引擎提交新的数据，根据缓冲区使用情况可能一定时间内会被阻塞
		void SubmitBuffer();
		
		//开始播放音频流中的数据
		void Run();
		//停止播放音频中的数据
		void Stop();
		//获得当前的播放状态
		int GetBufferSize() const;
		//at stop state
		void ChangeBufferInfo(int bufferNum, int bufferSize);

		~AudioEngine();
	private:
		static const int NoBuffer = -1;

		int mInited;
		int curFreeIdx;      //当前需要写入的空闲缓冲区序号，若没有则为-1
		int mBufferNum;       //循环缓冲中缓冲的个数
		int mBufferSize;		 //每个缓冲区的大小

		WAVEFORMATEX waveFormat;
		VoiceCallback callBack;
		const byte* inputBuffer;
		byte* bufferData;        //整个循环缓冲区的空间

		HRESULT hr;
		IXAudio2* mXAudio2;
		IXAudio2MasteringVoice* mMasterVoice;
		IXAudio2SourceVoice* mSourceVoice;
		
		byte* CurrentBufferPtr();
		MapleTime timeCounter;
	};
}


