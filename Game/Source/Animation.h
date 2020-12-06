#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_Rect.h"
#include "App.h"

#define MAX_FRAMES 25

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	float currentFrame;


private:

	int lastFrame = 0;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[lastFrame++] = rect;
	}
	bool Done()
	{
		if (currentFrame == lastFrame - 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int GetCurrentFrameIndex()
	{
		return currentFrame;
	}

	SDL_Rect& GetCurrentFrame()
	{
		currentFrame += speed;
		if (currentFrame >= lastFrame)
		{
			currentFrame = (loop) ? 0.0f : lastFrame - 1;
			loops++;
		}

		return frames[(int)currentFrame];
	}

	void Reset()
	{
		currentFrame = 0;
	}

	bool Finished() const
	{
		return loops > 0;
	}
};

#endif