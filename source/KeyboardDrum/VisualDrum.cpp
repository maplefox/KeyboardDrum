#include "Common.h"
#include "VisualDrum.h"


VisualDrum::VisualDrum(int eventNum,Camera* camera,Particles* particles,GraphicsEngine* graphicsEngine)
	:mBackIndex(0),mTimeCounter(), mEventBuffers{ vector<EventInfo>(eventNum),vector<EventInfo>(eventNum) },
	mCamera(camera), mParticles(particles), mGraphicsEngine(graphicsEngine)
{
	mFrontBuffer = &mEventBuffers[mBackIndex];
	mBackIndex = (mBackIndex + 1) % 2;
	mBackBuffer = &mEventBuffers[mBackIndex];
	mWindowResized = false;
}


VisualDrum::~VisualDrum()
{
	delete mParticles;
	delete mCamera;
	delete mGraphicsEngine;
}

void VisualDrum::Init(HINSTANCE instance, HWND window)
{
	auto view = mCamera->View();
	auto proj = mCamera->Projection();
	auto particles = mParticles->ParticleOutput();
	mCamera->Initialize(&mFrontBuffer);
	mParticles->Init(&mFrontBuffer);
	mGraphicsEngine->Initialize(instance, window, particles,view,proj);
}

void VisualDrum::SwapBuffers()
{
	mFrontBuffer = &mEventBuffers[mBackIndex];
	mBackIndex = (mBackIndex+1)%2;
	auto backBuffer = &mEventBuffers[mBackIndex];
	for (auto& event : *backBuffer)
	{
		event = { 0 };
	}
	mBackBuffer = backBuffer;
}
/*
void VisualDrum::ResizeWindow(int width, int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
	mWindowResized = true;
}
*/
void VisualDrum::MainExecute()
{

	while (!mStopSignal)
	{
		float lapsedTime = mTimeCounter.Update();

		//if(mWindowResized)
		

		SwapBuffers();
		mCamera->Update(lapsedTime);
		mParticles->Update(lapsedTime);
		mGraphicsEngine->Draw();
	}
	
}


void VisualDrum::Run()
{
	mRunning = true;
	mStopSignal = false;
	mRunThread = thread(&VisualDrum::MainExecute, this);
	
}

void VisualDrum::Stop()
{
	
	if (!mRunning)
		return;
	mStopSignal = true;
	mRunThread.join();
	
}
const EventsPtr* VisualDrum::EventInput()
{
	return &mBackBuffer;
}

