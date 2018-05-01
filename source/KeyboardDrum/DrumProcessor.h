#pragma once
#include "TypeDef.h"
#include "AudioMixer.h"

namespace MapleDrum
{
	enum PlayState
	{
		Stopped = 0,
		Playing, Switching, Disappear
	};

	struct SoundMutex
	{
		int activeId;   //-1 if No sound is active
		vector<int> mutexIds;
	};
	struct AudioData
	{
		int blockNum;
		float* dataPtr;
	};


	struct DrumState
	{
		//setting
		int id;
		int audioId;
		int keyPlayId;
		int keyStopId;
		int mutexId;

		float volume;            //0-1
		int switchLength;   //change name to disappearLength	
		int disappearLength;
		int disappearDefer;


		//playstate
		int playState;       //stop playing switch disappear

		int curPos;
		int switchPos;
		int stopPos;         //-1代表一直播放到结束
		int shadeLength;
		float playVolume;



	};
	class DrumProcessor
	{
	public:
		
		DrumProcessor(int blockNum,	
					int channelNum,int mutexNum,
					vector<DrumState>& drumStates,	
					vector<AudioData>& audioDatas);
		~DrumProcessor();

		void ComputData();
		void StopAll();
		const float* AudioOutput();
		void Init(const vector<EventInfo>* keyEvents);
		void SetPlayEventBuffer(vector<EventInfo>* buffer);
		
		
	private:
		
		int mBlockNum;

		AudioMixer mAudioMixer;
		const vector<EventInfo>* mKeyEvents;
		

		
		vector<int> mDrumMutexes;   //-1 if no sound playing
		vector<DrumState> mDrumStates;
		vector<AudioData> mAudioDatas;
		
		
		void ProcessInput();
		void WriteAudio();

		void ContinuePlay(DrumState& drum);
		void DisappearPlay(DrumState& drum);
		void SwitchPlay(DrumState& drum);
	};



}
