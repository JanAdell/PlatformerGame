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
	LoadLevel1();

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
	
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT)
	{
		LoadLevel1();
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
	{
		LoadLevel2();
	}

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

void Scene::LoadLevel1()
{
	Lvl1;
	app->player->CleanUp();
	app->map->CleanUp();
	app->map->Load("level1.tmx");
	app->player->Start();
	app->audio->PlayMusic("Assets/audio/music/lvl1bgm.ogg");
}

void Scene::LoadLevel2()
{
	!Lvl1;
	app->player->CleanUp();
	app->fade->FadeTo();
	app->map->CleanUp();
	app->fade->FadeTo();
	app->map->Load("level2.tmx");
	app->player->Start();
	app->audio->PlayMusic("Assets/audio/music/lvl2bgm.ogg");
}