#pragma once

#include "Common.h"

#define DeleteObject(object) if((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects) if((objects) != NULL) { delete[] objects; objects = NULL; }
#define ReleaseObject(object) if((object) != NULL) { object->Release(); object = NULL; }

using namespace DirectX;
using namespace std;

namespace MapleDrum
{
	
	typedef unsigned char byte;
	typedef float* floatPtr;
	
	struct EventInfo
	{
		int type;   //how many times
		float value;
	};
	typedef vector<EventInfo>* EventsPtr;
	typedef const vector<EventInfo>* ConstEventsPtr;



	struct Particle
	{
		int vertexStart;
		int vertexNum;
		XMFLOAT4A color;
		XMFLOAT4X4A model;
	};

	inline float RandomFloat(float min, float max)
	{

		float normRand = float(rand()) / RAND_MAX;
		return (max - min)*normRand + min;
	}
	inline XMVECTOR RandomAxis()
	{
		float u, v, r2;
		do
		{
			u = RandomFloat(-1.0, 1.0);
			v = RandomFloat(-1.0, 1.0);
		} while ((r2 = u*u + v*v) > 1);

		float x = 2 * u*sqrt(1 - r2);
		float y = 2 * v*sqrt(1 - r2);
		float z = 1 - 2 * r2;
		return XMVectorSet(x, y, z, 0);
	}
	
}

