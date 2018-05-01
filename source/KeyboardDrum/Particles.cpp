#include "Common.h"
#include "Particles.h"



Particles::Particles( float hueWidth,float hueSpeed, const vector<Model>& models, const vector<ParticleEventDef>& eventDefs) :
	mHueWidth(hueWidth), mHueSpeed(hueSpeed),
	mEventDefs(eventDefs), mModels(models)
{	
	Reset();
}


Particles::~Particles()
{
}
void Particles::Init(const ConstEventsPtr* eventInput)
{
	mEventBuffer = eventInput;
}

void Particles::Update(float lapsedTime)
{
	
	SwapBuffers();
	mHuePhase += lapsedTime*mHueSpeed;
	mHuePhase -= int(mHuePhase);
	ProcessEvents();
	UpdateEmitters(lapsedTime);
	GenerateParticles();
}

void Particles::SwapBuffers()
{
	auto tmp = mFrontEmitters;
	mFrontEmitters = mBackEmitters;
	mBackEmitters = tmp;
}
//return float 0-1


void Particles::ProcessEvents()
{

	

	auto& eventBuffer = **mEventBuffer;
	auto& newEmitters = *mBackEmitters;
	for (auto& eventDef : mEventDefs)
	{
		if (!eventBuffer[eventDef.eventId].type)
			continue;
		Emitter emitter;
		emitter.lifetime = eventDef.lifetime;
		emitter.lifecycle = 1.0f;
		emitter.modelId = eventDef.modelId;
		emitter.objectNum = eventDef.objectNum;
		emitter.scale = eventDef.scale;
		
		
		emitter.hue = eventDef.hue;
		emitter.gamma = eventDef.gamma;
		emitter.brightness = eventDef.brightness;

		emitter.kDrag = eventDef.kDrag;
		emitter.spreadSpeed = eventDef.spreadSpeed;
		emitter.axisRotateSpeed = eventDef.axisRotateSpeed;
		emitter.selfRotateSpeed = eventDef.selfRotateSpeed;
		emitter.selfAngle = RandomFloat(0, XM_2PI);
		emitter.axisAngle = RandomFloat(0, XM_2PI);
		emitter.radius = eventDef.appearSt;
		
		

		float x = RandomFloat(-XM_PI, XM_PI);
		float y = RandomFloat(-XM_PIDIV2, XM_PIDIV2);
		float z = RandomFloat(-XM_PI, XM_PI);
		//auto axis = RandomAxis();
		//auto crossAxis = XMVector3Cross(XMVectorSet(0, 1, 0, 0), axis);

		//float crossAngle = asin(XMVectorGetX(XMVector3Length(crossAxis)));
		//crossAxis = XMVector3Normalize(crossAxis);
		//auto rotate = XMQuaternionRotationNormal(crossAxis, crossAngle);
		auto rotate = XMQuaternionRotationNormal(XMVectorSet(0, 1, 0, 0), -y);
		rotate = XMQuaternionMultiply(rotate, XMQuaternionRotationRollPitchYaw(y, x, 0));
		XMStoreFloat4(&emitter.axis, rotate);
		newEmitters.push_back(emitter);



	}
}

void Particles::UpdateEmitters(float lapsedTime)
{

	

	auto& frontEmitters = *mFrontEmitters;
	auto& backEmitters = *mBackEmitters;
	frontEmitters.clear();
	for (auto& emitter : backEmitters)
	{
		
		emitter.lifecycle -= lapsedTime/emitter.lifetime;
		if (emitter.lifecycle <0)
			continue;
		auto dragRate = 1 - emitter.kDrag*lapsedTime/emitter.lifetime;
		emitter.spreadSpeed *= dragRate;
		emitter.selfRotateSpeed *= dragRate;
		emitter.axisRotateSpeed *= dragRate;
		emitter.radius += emitter.spreadSpeed * lapsedTime;
		emitter.axisAngle += emitter.axisRotateSpeed*lapsedTime;
		if (emitter.axisAngle > XM_2PI) emitter.axisAngle -= XM_2PI;
		emitter.selfAngle += emitter.selfRotateSpeed*lapsedTime;
		if (emitter.selfAngle > XM_2PI) emitter.selfAngle -= XM_2PI;
		frontEmitters.push_back(emitter);
	}
}

void Particles::GenerateParticles()
{
	mParticles.clear();
	auto& frontEmitters = *mFrontEmitters;
	for (auto& emitter : frontEmitters)
	{
		Particle particle;
		auto& model = mModels[emitter.modelId];
		particle.vertexNum = model.vertexNum;
		particle.vertexStart = model.vertexStart;
		float brightness = emitter.brightness * pow(emitter.lifecycle, emitter.gamma);
		float hue = emitter.hue*mHueWidth + mHuePhase;
		hue -= int(hue);
		XMVECTOR hsv = XMVectorSet(hue, 1.0, 1.0, 1.0);
		XMStoreFloat4A(&particle.color, XMColorHSVToRGB(hsv));
		particle.color.w = brightness;
		for (int i = 0; i < emitter.objectNum; i++)
		{
			float objAnble = emitter.axisAngle + float(i) / emitter.objectNum * XM_2PI;   //¹«×ª½Ç¶È
			float selfAngle = emitter.selfAngle + float(i) / emitter.objectNum * XM_2PI;
			XMMATRIX model = XMMatrixIdentity();
			model *= XMMatrixRotationZ(selfAngle);
			model *= XMMatrixScaling(emitter.scale, emitter.scale, emitter.scale);
			model *= XMMatrixTranslation(0 , 0 , emitter.radius);
			
			model *= XMMatrixRotationY(objAnble);
			model *= XMMatrixRotationQuaternion(XMLoadFloat4(&emitter.axis));
			XMStoreFloat4x4A(&particle.model, model);
			mParticles.push_back(particle);

		}
	}
	//printf("\r emitterNum:%d, particlenum:%d",frontEmitters.size(), mParticles.size());
}

const vector<Particle>* Particles::ParticleOutput() const
{
	return &mParticles;
}

void Particles::Reset()
{
	mHuePhase = 0;
	mFrontEmitters = &mEmitters[0];
	mBackEmitters = &mEmitters[1];
	mEmitters[0].clear();
	mEmitters[1].clear();
	mParticles.clear();
}