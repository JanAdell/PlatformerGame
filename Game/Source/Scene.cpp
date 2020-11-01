#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "FadetoBlack.h"
#include "Defs.h"
#include "Log.h"

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
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1;
	
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT && intro)
		LoadLevel1();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT)
	{
		LoadLevel1();
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
	{
		LoadLevel2();
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->LoadGame();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->SaveGame();


	//app->render->DrawTexture(img, 380, 100);
	app->map->Draw();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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
	app->player->CleanUp();
	app->fade->FadeTo();
	app->map->CleanUp();
	app->fade->FadeTo();
	app->player->Start();
	if (Lvl = 1)
	{
		app->map->Load("level1.tmx");
		app->audio->PlayMusic("Assets/audio/music/lvl1bgm.ogg");
		Lvl = 2;
	}
	else
	{
		app->map->Load("level2.tmx");
		app->audio->PlayMusic("Assets/audio/music/lvl2bgm.ogg");
		Lvl = 1;
	}
}

void Scene::LoadLevel1()
{
	Lvl = 1;
	app->player->CleanUp();
	app->map->CleanUp();
	app->map->Load("level1.tmx");
	app->player->Start();
	app->audio->PlayMusic("Assets/audio/music/lvl1bgm.ogg");
}

void Scene::LoadLevel2()
{
	Lvl = 2;
	app->player->CleanUp();
	app->fade->FadeTo();
	app->map->CleanUp();
	app->fade->FadeTo();
	app->map->Load("level2.tmx");
	app->player->Start();
	app->audio->PlayMusic("Assets/audio/music/lvl2bgm.ogg");
}

void Scene::LoadIntro()
{
	intro;
	//app->player->CleanUp();
	app->fade->FadeTo();
	app->map->CleanUp();
	app->fade->FadeTo();
	app->map->Load("intro.tmx");
	app->player->Start();
	app->audio->PlayMusic("Assets/audio/music/lvl2bgm.ogg");
}

bool Scene::Save(pugi::xml_node& data)const
{
	bool ret = true;
	pugi::xml_node lvl_stats = data.append_child("lvl_stats");
	lvl_stats.append_attribute("level") = Lvl;
	return ret;
}
bool Scene::Load(pugi::xml_node& data)
{
	bool ret = true;
	pugi::xml_node lvl_stats = data.child("lvl_stats");
	if (Lvl != lvl_stats.attribute("level").as_uint())
		Lvl = lvl_stats.attribute("level").as_uint();

	LoadLevel();
	return ret;
}
