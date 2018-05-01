#pragma once

#include "TypeDef.h"
#include "Particles.h"
#include "Camera.h"
#include "GraphicsEngine.h"
#include "MapleTime.h"

using namespace std;


namespace MapleDrum
{
	

	class VisualDrum
	{
	public:

		VisualDrum(int eventNum, Camera* camera, Particles* particles, GraphicsEngine* graphicsEngine);
		~VisualDrum();
		void ResizeWindow(int width, int height);
		void Run();
		void Stop();
		void Init(HINSTANCE instance, HWND window);
		void Release();
		const EventsPtr* EventInput();

	private:

		bool mRunning;
		bool mStopSignal;
		bool mWindowResized;
		thread mRunThread;
		Camera* mCamera;
		Particles* mParticles;
		GraphicsEngine* mGraphicsEngine;


		void MainExecute();
		MapleTime mTimeCounter;


		int mBackIndex;
		vector<EventInfo> mEventBuffers[2];

		EventsPtr mFrontBuffer;
		EventsPtr mBackBuffer;
		void SwapBuffers();

		int curEmitterIndex;



	};
}


