#include "App.h"
#include "Window.h"
#include <math.h>
#include "Log.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

FadeToBlack::FadeToBlack()
{
	name.create("FadeToBlack");

}
FadeToBlack::~FadeToBlack()
{
}

bool FadeToBlack::Start()
{
	LOG("Starting Fade.");
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	screen = { 0, 0, (int)app->win->width, (int)app->win->height };

	return true;
}

bool FadeToBlack::Update(float dt)
{
	if (currentStep == FadeStep::NONE)
		return true;

	Uint32 now = SDL_GetTicks() - startTime;
	float normalized = MIN(1.0f, (float)now / (float)totalTime);

	switch (currentStep)
	{
	case FadeStep::FADE_TO_BLACK:
	{
		if (now >= totalTime)
		{
			totalTime += totalTime;
			startTime = SDL_GetTicks();
			currentStep = FadeStep::FADE_FROM_BLACK;
		}
	} break;

	case FadeStep::FADE_FROM_BLACK:
	{
		normalized = 1.0f - normalized;

		if (now >= totalTime)
			currentStep = FadeStep::NONE;
	} break;
	}

	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(app->render->renderer, &screen);

	return true;
}

bool FadeToBlack::CleanUp()
{
	return true;
}

bool FadeToBlack::FadeTo(float time)
{
	bool ret = false;

	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::FADE_TO_BLACK;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5f * 1000.0f);

		ret = true;
	}

	return ret;
}
bool FadeToBlack::FadeFrom(float time)
{
	bool ret = false;

	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::FADE_FROM_BLACK;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5f * 1000.0f);

		ret = true;
	}

	return ret;
}

bool FadeToBlack::Fading() const
{
	return currentStep != FadeStep::NONE;
}