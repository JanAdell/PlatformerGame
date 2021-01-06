#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
//#include "SceneMenu.h"
#include "Player.h"
#include "FadetoBlack.h"
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"
#include "Collisions.h"
#include "Pathfinding.h"
#include "GuiButton.h"

Scene::Scene() : Module()
{
	name.create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->win->SetTitle("Platformer Game: Charged v0.1");
	LoadIntro();

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && intro)
		LoadLevel1();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		LoadLevel1();
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		LoadLevel2();
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->entityManager->player->position.x = dynamic_cast<Player*>(app->entityManager->player)->spawnPos.x;
		app->entityManager->player->position.y = dynamic_cast<Player*>(app->entityManager->player)->spawnPos.y;
	}

	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN/* && intro == false*/)
	{
		app->entityManager->player->position.x = dynamic_cast<Player*>(app->entityManager->player)->checkpointPos.x;
		app->entityManager->player->position.y = dynamic_cast<Player*>(app->entityManager->player)->checkpointPos.y;
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGame();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGame();

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		app->godMode = !app->godMode;

	//app->render->DrawTexture(img, 380, 100);
	app->map->Draw();

	if (intro)
	{
		play->Update(dt);
		resume->Update(dt);
		settings->Update(dt);
		credits->Update(dt);
		quit->Update(dt);
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate(float dt)
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (intro)
	{
		play->Draw();
		resume->Draw();
		settings->Draw();
		credits->Draw();
		quit->Draw();
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Scene::LoadLevel()
{
	app->entityManager->player->checkpoint = false;
	app->godMode = false;
	app->entityManager->CleanUp();
	app->fade->FadeTo();
	app->map->CleanUp();
	app->fade->FadeTo();
	app->entityManager->Start();
	if (lvl == 1)
	{
		app->map->Load("level1.tmx");
		app->audio->PlayMusic("Assets/audio/music/lvl1bgm.ogg");
		lvl = 2;
	}
	else
	{
		app->map->Load("level2.tmx");
		app->audio->PlayMusic("Assets/audio/music/lvl2bgm.ogg");
		lvl = 1;
	}
}

void Scene::LoadLevel1()
{
	app->pause = true;
	app->godMode = false;
	lvl = 1;
	intro = false;
	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->map->CleanUp();
	app->map->Load("level1.tmx");
	app->entityManager->Start();
	app->audio->PlayMusic("Assets/audio/music/lvl1bgm.ogg");
	
}

void Scene::LoadLevel2()
{
	app->pause = true;
	app->godMode = false;
	lvl = 2;
	intro = false;
	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	//app->fade->FadeTo();
	app->map->CleanUp();
	//app->fade->FadeTo();
	app->map->Load("level2.tmx");
	app->entityManager->Start();
	app->audio->PlayMusic("Assets/audio/music/lvl2bgm.ogg");
	
}

void Scene::LoadIntro()
{
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
}

bool Scene::Save(pugi::xml_node& data)const
{
	bool ret = true;
	pugi::xml_node lvl_stats = data.append_child("lvl_stats");
	lvl_stats.append_attribute("level") = lvl;
	return ret;
}
bool Scene::Load(pugi::xml_node& data)
{
	bool ret = true;
	pugi::xml_node lvl_stats = data.child("lvl_stats");
	if (lvl != lvl_stats.attribute("level").as_uint())
		lvl = lvl_stats.attribute("level").as_uint();

	if (lvl == 1)
	{
		LoadLevel1();
	}
	else LoadLevel2();
	return ret;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1)
			LoadLevel1();
		/*else if (control->id == 2)
			//Load();
		else if (control->id == 3)
		else if (control->id == 4)
		else if (control->id == 5)*/
	default:
		break;
	}

	return true;
	}
}