#pragma once
#include "TypeDef.h"

using namespace MapleDrum;

struct Model
{
	int vertexStart;
	int vertexNum;
};

struct Emitter
{
	float lifetime;
	float lifecycle;
	int modelId;
	int objectNum;
	
	float hue;
	float gamma;
	float brightness;
	float scale;
	float kDrag;
	float spreadSpeed;
	float radius;
	float axisRotateSpeed;
	float selfRotateSpeed;
	float selfAngle;
	float axisAngle;
	XMFLOAT4 axis;
	
};


struct ParticleEventDef
{
	int eventId;
	int modelId;
	int objectNum;
	float hue;
	float gamma;
	float brightness;
	float scale;
	float kDrag;
	float lifetime;
	float spreadSpeed;
	float axisRotateSpeed;
	float selfRotateSpeed;
	float appearSt;
};

class Particles
{
public:
	Particles(float hueWidth, float hueSpeed, const vector<Model>& models, const vector<ParticleEventDef>& eventDefs);
	~Particles();
	void Init(const ConstEventsPtr* eventInput);
	void Update(float lapsedTime);
	const vector<Particle>* ParticleOutput() const;
	void Reset();
private:
	
	float mHuePhase;
	float mHueWidth;
	float mHueSpeed;

	const ConstEventsPtr* mEventBuffer;
	vector<Model> mModels;
	vector<ParticleEventDef> mEventDefs;
	vector<Emitter> mEmitters[2];
	vector<Emitter>* mFrontEmitters;
	vector<Emitter>* mBackEmitters;
	vector<Particle> mParticles;
	void SwapBuffers();
	void ProcessEvents();
	void UpdateEmitters(float lapsedTime);
	void GenerateParticles();
	
	
};

