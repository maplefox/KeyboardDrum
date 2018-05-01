#include "Common.h"
#include "Camera.h"




Camera::Camera(float fov, float aspectRatio,  float radius, float drag, 
	float regress,float slope, vector<MovEvent>& movEvtDefs):
	mFovAngleY(fov),mNearZ(0.1f),mFarZ(1000.0f),mAspectRatio(aspectRatio),
	mRadius(radius),mDrag(drag) ,mRegress(regress),mSlope(slope),
	mMovEvents(movEvtDefs)
{
	Reset();
	
}


Camera::~Camera()
{
}

void Camera::Initialize(const ConstEventsPtr* eventBuffer)
{
	mEventBuffer = eventBuffer;
}

void Camera::Update(float time)
{
	

	UpdateMovs(time);
	//UpdateVibs(time);
	ComputMatrix();
}

void Camera::ComputMatrix()
{
	auto view = XMMatrixIdentity();
	view *= XMMatrixRotationZ(roll*XM_2PI);
	view *= XMMatrixRotationQuaternion(XMLoadFloat4(&mViewRotate));
	//view *= XMMatrixTranslationFromVector(XMLoadFloat3(&mVibPos));
	view *= XMMatrixTranslation(0, 0, mRadius);
	auto rotate = XMLoadFloat4(&mRotate);
	view *= XMMatrixRotationQuaternion(rotate);
	
	view = XMMatrixInverse(nullptr, view);
	
	XMStoreFloat4x4(&mView, view);
	auto proj = XMLoadFloat4x4(&mProjection);

	auto viewProj = view*proj;
	XMStoreFloat4x4(&mViewProj, viewProj);
}

void Camera::Reset()
{

	auto proj = XMMatrixPerspectiveFovRH(mFovAngleY, mAspectRatio, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProjection, proj);
	movVel = { 0,0 };
	mVibPos = { 0,0,0 };
	dir = XM_PIDIV2;
	roll = 0;
	rollVel = 0;
	XMStoreFloat4(&mRotate, XMQuaternionIdentity());
	mFrontVibs = &mVibs[0];
	mBackVibs = &mVibs[1];
	mRotVel = { 0,0 };
	mRotPos = { 0,0 };
}

const XMFLOAT4X4* Camera::ProjView()
{
	return &mViewProj;
}

const XMFLOAT4X4* Camera::Projection()
{
	return &mProjection;
}


const XMFLOAT4X4* Camera::View()
{
	return &mView;
}


void Camera::UpdateVibs(float time)
{
	auto tmp = mFrontVibs;
	mFrontVibs = mBackVibs;
	mBackVibs = tmp;
	auto& eventBuffer = **mEventBuffer;
	auto& frontVibs = *mFrontVibs;
	auto& backVibs = *mBackVibs;
	frontVibs.clear();

	for (auto& vibDef : mVibDefs)
	{
		if (eventBuffer[vibDef.eventId].type)
		{
			backVibs.push_back(vibDef.vib);
		}
	}
	
	auto vibPos = XMVectorZero();

	
	for (auto& vib : backVibs)
	{
		vib.amplitude *= pow(vib.decayRate, time);
		if (vib.amplitude < vib.endAmp)
			continue;
		vib.phase += vib.frequency * time;
		vib.phase -= int(vib.phase);
		frontVibs.push_back(vib);
		//printf("%d  ", frontVibs.size());
		float x = vib.amplitude*sin(XM_2PI* vib.phase);

		vibPos += x* XMLoadFloat3(&vib.direction);
	}

	XMStoreFloat3(&mVibPos, vibPos);

}



void Camera::UpdateMovs(float time)
{
	auto events = **mEventBuffer;

	auto vel = XMLoadFloat2(&movVel);
	
	auto rotVel = XMLoadFloat2(&mRotVel);
	auto rotPos = XMLoadFloat2(&mRotPos);
	for (auto& evtDef : mMovEvents)
	{
		auto& evt = events[evtDef.eventId];
		if (evt.type)
		{
			float rad = evtDef.rad;// +dir;
			float x = cos(rad);
			float y = sin(rad);
			auto acc = evtDef.power*XMVectorSet(x, y, 0, 0);
			vel += acc;
			//rotVel += acc;
			rollVel += evtDef.rollAcc;
			
		}
	}
	auto decayRate = 1 - mDrag *time;
	vel *= decayRate;

	rotVel += mSlope*vel*time;
	rotVel *= decayRate;
	rotVel -= mRegress*rotPos*time;
	rotPos += rotVel*time;
	
	rollVel *= decayRate;

	roll += rollVel*time;
	roll -= int(roll);
	dir = atan2(rotPos.m128_f32[1], rotPos.m128_f32[0]);

	auto rotate = XMLoadFloat4(&mRotate);
	auto rotInc = XMQuaternionRotationRollPitchYaw(-movVel.y*time, movVel.x*time, 0);
	rotate = XMQuaternionMultiply(rotInc, rotate);
	auto length = XMVector2Length(rotPos).m128_f32[0];
	auto rotView = XMQuaternionRotationNormal(XMVectorSet(0, 0, 1, 0), -dir);

	rotView = XMQuaternionMultiply(rotView,XMQuaternionRotationNormal(XMVectorSet(0, 1, 0, 0), length));
	
	rotView = XMQuaternionMultiply(rotView, XMQuaternionRotationNormal(XMVectorSet(0, 0, 1, 0), dir)) ;
	

	XMStoreFloat4(&mRotate, rotate);
	XMStoreFloat4(&mViewRotate, rotView);
	XMStoreFloat2(&movVel, vel);
	XMStoreFloat2(&mRotPos, rotPos);
	XMStoreFloat2(&mRotVel, rotVel);

}