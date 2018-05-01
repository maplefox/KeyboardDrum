#include "Common.h"
#include "DrumProcessor.h"

using namespace MapleDrum;


DrumProcessor::DrumProcessor(int blockNum,	int channelNum,	int mutexNum,
	vector<DrumState>& drumStates,	vector<AudioData>& audioDatas):
	mBlockNum(blockNum), mAudioMixer(channelNum,blockNum),
	mDrumMutexes(mutexNum), mDrumStates(drumStates),mAudioDatas(audioDatas)
{

}


DrumProcessor::~DrumProcessor()
{
}

const float* DrumProcessor::AudioOutput()
{
	return mAudioMixer.Output();
}



inline int Min(int a, int b)
{
	return a < b ? a : b;
}

inline int Max(int a, int b)
{
	return a > b ? a : b;
}
inline float MinF(float a, float b)
{
	return a < b ? a : b;
}
inline float MaxF(float a, float b)
{
	return a > b ? a : b;
}

void DrumProcessor::Init(const vector<EventInfo>* keyEvents)
{
	mKeyEvents = keyEvents;

}

void DrumProcessor::StopAll()
{
	for (auto& drum : mDrumStates)
	{
		drum.playState = Stopped;
	}
}

void DrumProcessor::ProcessInput()
{
	for (auto& mutex : mDrumMutexes)
	{
		mutex = -1;
	}
	auto& keyEvents = *mKeyEvents;
	
	for (auto& drum : mDrumStates)
	{
		auto& audio = mAudioDatas[drum.audioId];
		
		

		bool toPlay = drum.keyPlayId != -1 && keyEvents[drum.keyPlayId].type;
		bool toStop = drum.keyStopId != -1 && keyEvents[drum.keyStopId].type;
		bool mutexed = false;
		if (drum.mutexId != -1)
		{
			auto& mutex = mDrumMutexes[drum.mutexId];
			toPlay = toPlay && mutex == -1;
			mutexed = mutex != -1;
		}
		
		if (toPlay)
		{


			drum.playVolume = drum.volume * keyEvents[drum.keyPlayId].value;

			switch (drum.playState)
			{
			case Switching:
			case Playing:
				drum.playState = Switching;
				drum.shadeLength = drum.switchLength;
				drum.stopPos = Min(drum.curPos + drum.shadeLength, audio.blockNum);
				drum.switchPos = drum.curPos;
				drum.curPos = 0;
				break;

			case Disappear:
			case Stopped:
				drum.playState = Playing;
				drum.stopPos = audio.blockNum;
				drum.curPos = 0;
				break;
			}
			if(drum.mutexId !=  -1)
				mDrumMutexes[drum.mutexId] = drum.id;

			
		}

		if (drum.playState == Playing || drum.playState == Switching)
		{

			if (mutexed)
			{
				drum.playState = Disappear;
				drum.switchLength = drum.curPos;
				drum.shadeLength = drum.switchLength;
				drum.stopPos = Min(audio.blockNum, drum.curPos + drum.shadeLength);
			}
			else if (toStop)
			{
				drum.shadeLength = drum.disappearLength;
				drum.stopPos = Min(audio.blockNum, drum.curPos + drum.disappearDefer);
			}
			
		}


	}


	
}

void DrumProcessor::ComputData()
{
	
	mAudioMixer.ClearBuffer();
	ProcessInput();
	WriteAudio();
}

void DrumProcessor::WriteAudio()
{
	for (auto& drum : mDrumStates)
	{
		switch (drum.playState)
		{
		case Playing:
			ContinuePlay(drum);
			break;
		case Switching:
			SwitchPlay(drum);
			break;
		case Disappear:
			DisappearPlay(drum);
			break;
		}
	}
	
}

void DrumProcessor::ContinuePlay(DrumState& drum)
{

	auto& audio = mAudioDatas[drum.audioId];
	int endPos = drum.curPos + mBlockNum;

	int toWrite = Min(endPos, drum.stopPos) - drum.curPos;
	mAudioMixer.ConstantMix(audio.dataPtr, drum.curPos, drum.playVolume, toWrite);
	drum.curPos += toWrite;
	if (drum.curPos < endPos)
	{
		drum.switchPos = drum.curPos;
		drum.shadeLength = drum.disappearLength;
		drum.stopPos = Min(audio.blockNum, drum.switchPos + drum.shadeLength);
		toWrite = Min(endPos, drum.stopPos) - drum.curPos;
		float t = float(toWrite) / drum.shadeLength;
		mAudioMixer.VariableMix(audio.dataPtr, drum.curPos, drum.playVolume, (1 - t)*drum.playVolume, toWrite);
		drum.curPos += toWrite;
		if (drum.curPos < endPos)
			drum.playState = Stopped;
		else
			drum.playState = Disappear;

	}

	//int toWrite = Min(blockNum, sound.stopPos - sound.curPos);
	//mAudioMixer.ConstantMix(sound.dataPtr, sound.curPos, sound.volume,toWrite);
	//sound.curPos += blockNum;
	//if (toWrite<blockNum)    //sound.curPos >= sound.stopPos
	//{
	//	if (sound.stopPos < sound.blockNum)
	//	{
	//		
	//		sound.switchPos = sound.curPos;
	//		toWrite = Min(blockNum - toWrite, sound.blockNum - sound.curPos);
	//		float t = float(toWrite) / sound.switchLength;
	//		mAudioMixer.VariableMix(sound.dataPtr, sound.curPos, sound.volume, (1 - t)*sound.volume, toWrite);
	//		sound.curPos += toWrite;
	//		if (sound.curPos >= sound.blockNum)
	//			sound.playState = Stopped;
	//		else
	//			sound.playState = Disappear;
	//	}
	//		
	//	else
	//		sound.playState = Stopped;//stop;
	//	
	//}
}

void DrumProcessor::DisappearPlay(DrumState& drum)
{
	auto& audio = mAudioDatas[drum.audioId];
	int endPos = drum.curPos + mBlockNum;
	int toWrite = Min(drum.stopPos, endPos) - drum.curPos;
	//int toWrite = blockNum - Max(endPos - drum.blockNum, 0);
	int progress = drum.curPos - drum.switchPos;
	float v1 = float(progress) / drum.shadeLength;
	float v2 = float(progress + toWrite) / drum.shadeLength;

	mAudioMixer.VariableMix(audio.dataPtr, drum.curPos, drum.playVolume*(1 - v1), drum.playVolume*(1 - v2), toWrite);
	drum.curPos += toWrite;
	if (drum.curPos < endPos)
		drum.playState = Stopped;

	/*int toWrite = Min(drum.blockNum - drum.curPos, blockNum);
	int progress = drum.curPos - drum.switchPos;
	toWrite = Min(toWrite, drum.switchLength - progress);
	float v1 = float(progress) / drum.switchLength;
	float v2 = float(progress + toWrite) / drum.switchLength;
	mAudioMixer.VariableMix(drum.dataPtr, drum.curPos, drum.volume*(1 - v1), drum.volume*(1 - v2), toWrite);
	drum.curPos += toWrite;
	if (toWrite <= blockNum)
	{
	drum.playState = Stopped;
	}*/
}

void DrumProcessor::SwitchPlay(DrumState& drum)
{
	auto& audio = mAudioDatas[drum.audioId];
	int curPos2 = drum.curPos + drum.switchPos;
	int toWrite = Min(drum.stopPos - curPos2, mBlockNum);
	float v1 = (float)drum.curPos / drum.shadeLength;
	float v2 = float(drum.curPos + toWrite) / drum.shadeLength;
	//printf("%f %f\n", v1, v2);
	mAudioMixer.VariableMix(audio.dataPtr, curPos2, drum.playVolume * (1 - v1), drum.playVolume * (1 - v2), toWrite);
	mAudioMixer.ConstantMix(audio.dataPtr, drum.curPos, drum.playVolume, mBlockNum);
	drum.curPos += mBlockNum;
	if (toWrite < mBlockNum)
	{
		drum.playState = Playing;
		drum.stopPos = audio.blockNum;
	}

}

