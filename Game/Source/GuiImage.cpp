#include "GuiImage.h"
#include "App.h"
#include "Render.h"
#include "GuiControl.h"


GuiImage::GuiImage(int pos_x, int pos_y, const SDL_Rect& r, GuiControl* parent, bool drawable) :GuiControl(pos_x, pos_y, parent, r.w, r.h, drawable)
{
	//ui_type = GuiControl::Type::IMAGE;
}

GuiImage::~GuiImage()
{
}

void GuiImage::CleanUp()
{

}
void GuiImage::InnerDraw()
{
	

}
#pragma once
