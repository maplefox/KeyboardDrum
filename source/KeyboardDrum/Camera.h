#pragma once

#include "TypeDef.h"

using namespace MapleDrum;


struct VibInfo
{
	float frequency;
	float phase;
	float amplitude;
	float decayRate;
	float endAmp;
	XMFLOAT3 direction;
};
struct VibDef
{
	int eventId;
	VibInfo vib;
};
struct MovEvent
{
	int eventId;
	float rad;
	float power;
	float rollAcc;
	
};
class Camera
{
public:
	Camera(float fov, float aspectRatio, float radius, float drag, 
		float regress, float slope, vector<MovEvent>& movEvtDefs);
	~Camera();
	
	void Initialize(const ConstEventsPtr* eventBuffer);
	void Update(float time);
	const XMFLOAT4X4* ProjView();
	const XMFLOAT4X4* View();
	const XMFLOAT4X4* Projection();
	void Reset();
private:
	float mAspectRatio;
	float mFovAngleY;
	float mNearZ, mFarZ;
	
	XMFLOAT4X4 mProjection;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mViewProj;
	
	const ConstEventsPtr* mEventBuffer;
	vector<MovEvent> mMovEvents;

	XMFLOAT3 mVibPos;
	
	vector<VibDef> mVibDefs;
	vector<VibInfo> mVibs[2];
	vector<VibInfo>* mFrontVibs;
	vector<VibInfo>* mBackVibs;

	float mRadius;
	float mDrag;
	float mSlope;
	float mRegress;

	XMFLOAT2 movVel;
	XMFLOAT3 movAcc;

	XMFLOAT2 mRotVel;
	XMFLOAT2 mRotPos;

	//XMFLOAT3 rotVel;
	//XMFLOAT3 rotAcc;
	//XMFLOAT4 rotPos;

	XMFLOAT4 mRotate;
	XMFLOAT4 mViewRotate;

	float rollVel;
	float roll;
	float dir;
	void ComputMatrix();
	void UpdateMovs(float time);
	void UpdateVibs(float time);
};

