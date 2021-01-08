#ifndef _GUIIMAGE_H
#define _GUIIMAGE_H

#include "GuiControl.h"

class GuiImage :public GuiControl {
public:
	GuiImage(int pos_x, int pos_y, const SDL_Rect& r, GuiControl* parent, bool drawable = true);
	~GuiImage();
	void CleanUp();
	void InnerDraw();
	SDL_Rect dimension;
};

#endif 