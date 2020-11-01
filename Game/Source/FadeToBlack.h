#ifndef __FADETOBLACK_H
#define __FADETOBLACK_H


#include "Module.h"
#include "SDL/include/SDL_Rect.h"

class FadeToBlack : public Module
{
public:

	FadeToBlack();
	~FadeToBlack();

	bool Start();
	bool Update(float dt);
	bool CleanUp();
	bool FadeTo(float time = 1.0f);
	bool FadeFrom(float time = 1.0f);
	bool Fading() const;
private:

	enum FadeStep
	{
		NONE,
		FADE_TO_BLACK,
		FADE_FROM_BLACK
	} currentStep = FadeStep::NONE;

	Uint32 startTime = 0;
	Uint32 totalTime = 0;
	SDL_Rect screen;
	Module* toEnable = nullptr;
	Module* toDisable = nullptr;
};

#endif