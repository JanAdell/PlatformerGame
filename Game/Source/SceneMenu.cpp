/*#include "SceneMenu.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "FadeToBlack.h"
#include "GuiButton.h"
#include "Map.h"
#include "EntityManager.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"

SceneMenu::SceneMenu() : Module()
{
	name.create("menu");

	play = new GuiButton(1, { 640, 150, 300, 80 }, "START");
	play->SetObserver(this);
	
	resume = new GuiButton(2, { 640, 250, 300, 80 }, "RESUME");
	resume->SetObserver(this);
	
	settings = new GuiButton(3, { 640, 350, 300, 80 }, "SETTINGS");
	settings->SetObserver(this);

	credits = new GuiButton(4, { 640, 450, 300, 80 }, "CREDITS");
	credits->SetObserver(this);
	
	quit = new GuiButton(5, { 640, 550, 300, 80 }, "EXIT");
	quit->SetObserver(this);
}

SceneMenu::~SceneMenu(){}

bool SceneMenu::Awake()
{
	return true;
}

bool SceneMenu::Start()
{
	bool ret = true;
	app->pause = true;
	app->entityManager->CleanUp();
	//app->fade->FadeTo();
	app->map->CleanUp();
	//app->fade->FadeTo();
	if (app->map->Load("intro.tmx")) {
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	app->entityManager->Start();
	app->audio->PlayMusic("Assets/audio/music/lvl2bgm.ogg");
	app->pause = false;

	return ret;
}

bool SceneMenu::PreUpdate(float dt)
{
	return true;
}

bool SceneMenu::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && app->scene->intro)
	{
		app->scene->LoadLevel1();
	}

	play->Update(dt);
	resume->Update(dt);
	settings->Update(dt);
	credits->Update(dt);
	quit->Update(dt);

	return ret;
}

bool SceneMenu::PostUpdate(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	play->Draw();
	resume->Draw();
	settings->Draw();
	credits->Draw();
	quit->Draw();

	return ret;
}*/

//bool SceneMenu::OnGuiMouseClickEvent(GuiControl* control)
//{
//	switch (control->type)
//	{
//	case GuiControlType::BUTTON:
//	{
//		/*if (control->id == 1)
//			app->fade->FadeTo(1);
//		else if (control->id == 2)
//			return false;*/
//
//	default:
//		break;
//	}
//
//	return true;
//	}
//}

/*bool SceneMenu::CleanUp()
{
	//app->tex->UnLoad(introText);
	return true;
}
*/