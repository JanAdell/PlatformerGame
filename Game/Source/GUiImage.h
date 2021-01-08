#ifndef _GUIIMAGE_H
#define _GUIIMAGE_H

#include "GuiControl.h"

class GuiImage :public GuiControl {
public:
	GuiImage(int id, SDL_Rect bounds, const char* text);
	~GuiImage();
	void CleanUp();
	void Draw(Render* render);
};

#endif
