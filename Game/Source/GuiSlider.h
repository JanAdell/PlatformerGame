#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

    GuiSlider(int id, SDL_Rect bounds, const char *text);
    virtual ~GuiSlider();

    bool Update(float dt);
    bool Draw();

    bool hover = true;
    bool click = true;

private:

    // GuiSlider specific properties
    // Maybe some animation properties for state change?
    SDL_Rect slider;
    int value;

    int minValue;
    int maxValue;

    int clickFx;
    int hoverFx;
};

#endif // __GUISLIDER_H__
