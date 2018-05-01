#include "Common.h"
#include "AudioMixer.h"





namespace MapleDrum
{

	AudioMixer::AudioMixer(int channelNum, int mixLength)
		:mMixLength(mixLength),mChannelNum(channelNum)
	{
		mBufferLength = mChannelNum*mMixLength;
		mixBuffer = new float[mBufferLength];
	}
	AudioMixer::~AudioMixer()
	{
		delete[] mixBuffer;
	}

	void AudioMixer::ClearBuffer()
	{
		ZeroMemory(mixBuffer, sizeof(float)*mBufferLength);
	}
	
	// 将混音结果以FLOAT PCM输出
	const float* AudioMixer::Output()
	{
		return mixBuffer;
	}


	void AudioMixer::ConstantMix(const float* source, int start, float volume, int blockNum)
	{
		
		source += start*mChannelNum;
		for (int i = 0; i < blockNum*mChannelNum; i++)
		{
			mixBuffer[i] += volume*source[i];
		}
	}
	
	void AudioMixer::VariableMix(const float* source, int start, float startVolume, float endVolume, int blockNum)
	{
		float norVol = startVolume;
		float difVol = (endVolume-startVolume) / mMixLength;
		source += start*mChannelNum;
		for (int i = 0; i < blockNum*mChannelNum; i++)
		{
			mixBuffer[i] += norVol*source[i];
			if (i%mChannelNum == 0)
				norVol += difVol;
		}
	}
	void AudioMixer::SetMixLength(int mixLength)
	{
		delete[] mixBuffer;
		mMixLength = mixLength;
		mBufferLength = mChannelNum*mMixLength;
		mixBuffer = new float[mBufferLength];
	}
	
}

