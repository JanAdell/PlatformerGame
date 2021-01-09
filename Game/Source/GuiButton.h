#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

    GuiButton(int id, SDL_Rect bounds, const char *text);
    virtual ~GuiButton();

    bool Update(float dt);
    bool Draw();
    bool hover = true;
    bool click = true;

private:

    // Gui Button specific properties
    // Maybe some animation properties for state change?

    int clickFx;
    int hoverFx;
    
};

#endif // __GUIBUTTON_H__
