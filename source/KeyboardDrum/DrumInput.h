#pragma once

#include "TypeDef.h"
#include"KeyboardInput.h"

namespace MapleDrum
{
	enum KeyAction
	{
		Press=0,Release,Down,Up
	};
	struct EventDef
	{
		int eventId;
		byte keycode;
		int keyAction;
		float value;
	};
	
	
	class DrumInput
	{
		
	public:
		struct InitInfo
		{
			int eventNum;
			vector<EventInfo>& eventsDefs;
			float doubleDiscount;
			float doubleJudge;
		};
		DrumInput(int eventNum,
			vector<EventDef>& eventsDefs,
			float doubleDiscount,
			float doubleJudge);
		~DrumInput();
		const vector<EventInfo>* EventsOutput() const;
		int EventNum() const;
		void Init(HINSTANCE instance, HWND window);
		void Update(float lapsedTime);

		
		
		/*bool InitDevice();
		void Release();*/
	private:
		bool mInited;
		int mEventNum;
		KeyboardInput mKeyboard;
		vector<EventInfo> mEvents;
		vector<EventDef> mEventsDefs;
		float mDoubleDiscount;
		float mDoubleJudge;
		
	};
}


