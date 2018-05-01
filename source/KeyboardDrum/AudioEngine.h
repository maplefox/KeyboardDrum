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
		//                                                        ������Ҫͨ�����������������Ч��
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
	
	//������ʽ������Ƶ�ĵײ�����
	//typedef unsigned char byte;
	class AudioEngine
	{
	public:
		AudioEngine(int bufferNum, int bufferSize,const WAVEFORMATEX& format);

		void Init(const byte* inputBuffer);
		void Release();

		//����Ƶ�����ύ�µ����ݣ����ݻ�����ʹ���������һ��ʱ���ڻᱻ����
		void SubmitBuffer();
		
		//��ʼ������Ƶ���е�����
		void Run();
		//ֹͣ������Ƶ�е�����
		void Stop();
		//��õ�ǰ�Ĳ���״̬
		int GetBufferSize() const;
		//at stop state
		void ChangeBufferInfo(int bufferNum, int bufferSize);

		~AudioEngine();
	private:
		static const int NoBuffer = -1;

		int mInited;
		int curFreeIdx;      //��ǰ��Ҫд��Ŀ��л�������ţ���û����Ϊ-1
		int mBufferNum;       //ѭ�������л���ĸ���
		int mBufferSize;		 //ÿ���������Ĵ�С

		WAVEFORMATEX waveFormat;
		VoiceCallback callBack;
		const byte* inputBuffer;
		byte* bufferData;        //����ѭ���������Ŀռ�

		HRESULT hr;
		IXAudio2* mXAudio2;
		IXAudio2MasteringVoice* mMasterVoice;
		IXAudio2SourceVoice* mSourceVoice;
		
		byte* CurrentBufferPtr();
		MapleTime timeCounter;
	};
}


