#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"
#include "SDL/include/SDL_timer.h"

class Timer
{

public:

	// Constructor
	// ---------------------------------------------
	Timer()
	{
		Start();
	}

	// ---------------------------------------------
	void Start()
	{
		startedAt = SDL_GetTicks();
	}
	// ---------------------------------------------
	Uint32 Read() const
	{
		return SDL_GetTicks() - startedAt;
	}
	// ---------------------------------------------
	float ReadSec() const
	{
		return (SDL_GetTicks() - startedAt) / 1000.0F;
	}
	Uint32 GetStartedAt()
	{
		return startedAt;
	}
	Uint32	startedAt;
private:

};

#endif //__TIMER_H__