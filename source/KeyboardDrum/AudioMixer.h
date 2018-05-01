#pragma once
namespace MapleDrum
{

	class AudioMixer
	{
	public:
		AudioMixer(int tChannelNum,int tMixLength);
		~AudioMixer();
		static const int MaxVolume = 100;
		
		void ClearBuffer();
		
		//void Output(short* outputBuffer);
		const float* Output();
		void ConstantMix(const float* source, int start, float volume,int blockNum);
		
		void VariableMix(const float* source, int start, float startVolume, float endVolume,int blockNum);
		void SetMixLength(int blockNum);
	private:
		const float maxValue = 1.0f;  //32m-1
		const float minValue = -1.0f;  //-32m
		//int* mixBuffer;
		int mChannelNum;
		int mMixLength;
		int mBufferLength;
		float* mixBuffer;
		
		
		inline float Clamp(float value)
		{
			if (value > maxValue)
				return maxValue;
			if (value < minValue)
				return minValue;
			return value;
		}
	

		
	};


}
