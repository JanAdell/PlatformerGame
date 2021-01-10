#include "App.h"
#include "Window.h"
#include "Render.h"

#include "EntityManager.h"
#include "Player.h"
#include "Collisions.h"

#include "Defs.h"
#include "Log.h"

#define VSYNC true
#define CAMERA_MARGE_L 150
#define CAMERA_MARGE_XL 330

Render::Render() : Module()
{
	name.create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;

}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");

	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = app->win->screenSurface->w;
		camera.h = app->win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	
	return true;
}

// Called each loop iteration
bool Render::PreUpdate(float dt)
{
	SDL_RenderClear(renderer);
	CameraPos();

	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate(float dt)
{
	
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);

	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

bool Render::IsOnCamera(const int& x, const int& y, const int& w, const int& h, const float& speed) const
{
	uint scale = app->win->GetScale();
	SDL_Rect rect = { x * scale / speed,y * scale,w * scale / speed,h * scale };
	SDL_Rect cam = { -camera.x * scale,-camera.y * scale,camera.w * scale / speed,camera.h * scale };

	return SDL_HasIntersection(&rect, &cam);
}

void Render::CameraPos()
{
	if (app->entityManager->player == nullptr)
		return;
	uint scale = app->win->GetScale();
	SDL_Rect rect = { dynamic_cast<Player*>(app->entityManager->player)->collider->rect.x * scale ,dynamic_cast<Player*>(app->entityManager->player)->collider->rect.y * scale ,dynamic_cast<Player*>(app->entityManager->player)->collider->rect.w * scale ,dynamic_cast<Player*>(app->entityManager->player)->collider->rect.h * scale };
	SDL_Rect cam = { -camera.x * scale ,-camera.y * scale ,camera.w * scale ,camera.h * scale };

	if (cam.x + cam.w - (float)CAMERA_MARGE_XL < rect.x + rect.w)
		cam.x = rect.x + rect.w + (float)CAMERA_MARGE_XL - cam.w;

	else if (cam.x + (float)CAMERA_MARGE_L > rect.x)
		cam.x = rect.x - (float)CAMERA_MARGE_L;

	if (cam.y + cam.h - (float)CAMERA_MARGE_L < rect.y + rect.h)
		cam.y = (rect.y + rect.h + (float)CAMERA_MARGE_L - cam.h);

	else if (cam.y + (float)CAMERA_MARGE_XL > rect.y)
		cam.y = (rect.y - (float)CAMERA_MARGE_XL);

	camera.x = -cam.x;
	camera.y = -cam.y;

}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, SDL_RendererFlip flip, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y) + y * scale;

	
	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}
	

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	
		

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if (use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawText(Font* font, const char* text, int x, int y, int size, int spacing, SDL_Color tint)
{
	bool ret = true;

	int length = strlen(text);
	int posX = x;

	float scale = (float)size / font->GetCharRec(32).h;

	SDL_SetTextureColorMod(font->GetTextureAtlas(), tint.r, tint.g, tint.b);

	for (int i = 0; i < length; i++)
	{
		SDL_Rect recGlyph = font->GetCharRec(text[i]);
		SDL_Rect recDest = { posX, y, (scale * recGlyph.w), size };

		SDL_RenderCopyEx(renderer, font->GetTextureAtlas(), &recGlyph, &recDest, 0.0, { 0 }, SDL_RendererFlip::SDL_FLIP_NONE);

		posX += ((float)recGlyph.w * scale + spacing);
	}

	return ret;
}
