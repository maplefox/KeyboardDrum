#include "Common.h"
#include "DrumInput.h"
using namespace MapleDrum;


DrumInput::DrumInput(int eventNum, vector<EventDef>& eventsDefs,
	float doubleDiscount, float doubleJudge) :
	mKeyboard(),mEventNum(eventNum),mEventsDefs(eventsDefs),mEvents(eventNum),
	mDoubleDiscount(doubleDiscount),mDoubleJudge(doubleJudge)
{

}


DrumInput::~DrumInput()
{
	
}

inline float Min(float a, float b)
{
	return a < b ? a : b;
}

void DrumInput::Update(float lapsedTime)
{
	mKeyboard.Update();
	int i = 0;
	for (auto& evt : mEvents)
	{
		evt.type = 0;
		evt.type = 0;
	}
	for (auto& evtDef :  mEventsDefs)
	{
		
		bool active =
			(evtDef.keyAction == Press && mKeyboard.KeyPress(evtDef.keycode)) ||
			(evtDef.keyAction == Release && mKeyboard.KeyRelease(evtDef.keycode));
		if (active)
		{
			mEvents[evtDef.eventId] = { 1,evtDef.value };
			
		}
	}
}

void DrumInput::Init(HINSTANCE instance, HWND window)
{
	
	mKeyboard.Init(instance, window);
	mInited = true;
}


const vector<EventInfo>* DrumInput::EventsOutput() const
{
	return &mEvents;
}

