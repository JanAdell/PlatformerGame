#include "GuiImage.h"
#include "App.h"
#include "Render.h"
#include "GuiControl.h"


GuiImage::GuiImage(int pos_x, int pos_y, const SDL_Rect& r, GuiControl* parent, bool drawable) :GuiControl(GuiControlType::IMAGE, id)
{
	
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
