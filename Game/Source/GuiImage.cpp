#include "GuiImage.h"
#include "App.h"
#include "Render.h"
#include "GuiControl.h"


GuiImage::GuiImage(int id, SDL_Rect bounds, const char* text) :GuiControl(GuiControlType::IMAGE, id)
{
	this->bounds = bounds;
	this->text = text;
}

GuiImage::~GuiImage()
{

}

void GuiImage::CleanUp()
{

}
void GuiImage::Draw(Render* render)
{
	app->render->DrawRectangle(bounds, 0, 255, 0, 255);

}
#pragma once
