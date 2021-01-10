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
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"
#include "Collisions.h"
#include "Pathfinding.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "Font.h"

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
	LoadGUI();
	uiFont = new Font("Assets/Fonts/Squarified.xml", app->tex);
	
	
	sprintf_s(playButton, 64, "Play");
	sprintf_s(continueButton, 64, "Continue");
	sprintf_s(resumeButton, 64, "Resume");
	sprintf_s(settingsButton, 64, "Settings");
	sprintf_s(creditsButton, 64, "Credits");
	sprintf_s(exitButton, 64, "Exit");
	sprintf_s(fullscreenButton, 64, "FullScreen");
	sprintf_s(vsyncButton, 64, "VSYNC");
	sprintf_s(musicVolButton, 64, "Music Volume");
	sprintf_s(sfxVolButton, 64, "Sound Eeffects Volume");
	sprintf_s(backButton, 64, "Back");
	sprintf_s(returnButton, 64, "Main Menu");
	
	

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
	
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && intro && app->scene->pauseGame == false)
		LoadLevel1();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && app->scene->pauseGame == false)
	{
		LoadLevel1();
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && app->scene->pauseGame == false)
	{
		LoadLevel2();
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && app->scene->pauseGame == false)
	{
		app->entityManager->player->position.x = dynamic_cast<Player*>(app->entityManager->player)->spawnPos.x;
		app->entityManager->player->position.y = dynamic_cast<Player*>(app->entityManager->player)->spawnPos.y;
	}

	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN/* && intro == false*/ && app->scene->pauseGame == false)
	{
		app->entityManager->player->position.x = dynamic_cast<Player*>(app->entityManager->player)->checkpointPos.x;
		app->entityManager->player->position.y = dynamic_cast<Player*>(app->entityManager->player)->checkpointPos.y;
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && app->scene->pauseGame == false)
		app->LoadGame();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && app->scene->pauseGame == false)
		app->SaveGame();

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && app->scene->pauseGame == false)
		app->godMode = !app->godMode;

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN && app->scene->pauseGame == false)
		LoadIntro();

	//app->render->DrawTexture(img, 380, 100);
	app->map->Draw();

	if (intro == true) UpdateMenu(dt);
	else if (settingsActive == true && intro == false) UpdateSettings(dt);
	else if (creditsActive == true && intro == false) UpdateCredits(dt);
	if (pauseGame == true && printPause == true) UpdatePause(dt);
	if (pauseGame == true && settingsPauseActive == true) UpdatePauseSettings(dt);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate(float dt)
{
	if (canPause == true)
	{
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && pauseGame == false) pauseGame = true;
		else if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && pauseGame == true) { pauseGame = false; settingsPauseActive = false; printPause = true; }
	}

	if (intro == true)
	{
		play->Draw();
		resume->Draw();
		settings->Draw();
		credits->Draw();
		quit->Draw();
	}

	if (intro == true) DrawMenu();
	else if (settingsActive == true && intro == false) DrawSettings();
	else if (creditsActive == true && intro == false) DrawCredits();
	if (pauseGame == true && printPause == true) DrawPause();
	if (pauseGame == true && settingsPauseActive == true) DrawPauseSettings();

	return playing;
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
	canPause = true;
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
	canPause = true;
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

void Scene::LoadGUI()
{
	//------------ MAIN MENU ---------------
	play = new GuiButton(1, { 550, 295, 200, 50 }, "Start");
	play->SetObserver(this);

	resume = new GuiButton(2, { 550, 365, 200, 50 }, "Resume");
	resume->SetObserver(this);

	pugi::xml_document data;
	pugi::xml_parse_result result = data.load_file("save.xml");

	if (result == true)
	{
		resume->state = GuiControlState::NORMAL;
	}
	else
	{
		resume->state = GuiControlState::DISABLED;
	}


	settings = new GuiButton(3, { 550, 435, 200, 50 }, "Settings");
	settings->SetObserver(this);

	credits = new GuiButton(4, { 550, 505, 200, 50 }, "Credits");
	credits->SetObserver(this);

	quit = new GuiButton(5, { 550, 575, 200, 50 }, "Exit");
	quit->SetObserver(this);


	//------------ SETTINGS ---------------
	musicVol = new GuiSlider(1, { 550, 295, 50, 28 }, "Music Volume");
	musicVol->SetObserver(this);

	fxVol = new GuiSlider(2, { 550, 365, 50, 28 }, "FX Volume");
	fxVol->SetObserver(this);

	fullscreen = new GuiCheckBox(1, { 700, 435, 50, 50 }, "Fullscreen");
	fullscreen->SetObserver(this);

	vsync = new GuiCheckBox(2, { 700, 505, 50, 50 }, "Vsync");
	vsync->SetObserver(this);

	back = new GuiButton(6, { 550, 575, 200, 50 }, "Exit");
	back->SetObserver(this);

	backPause = new GuiButton(10, { 550, 575, 200, 50 }, "Exit");
	backPause->SetObserver(this);

	//------------ CREDITS ---------------

	back = new GuiButton(6, { 550, 575, 200, 50 }, "Exit");
	back->SetObserver(this);

	//------------ PAUSE ---------------
	unpause = new GuiButton(7, { 550, 295, 200, 50 }, "Start");
	unpause->SetObserver(this);

	settingsPause = new GuiButton(8, { 550, 365, 200, 50 }, "Resume");
	settingsPause->SetObserver(this);

	backTitle = new GuiButton(9, { 550, 435, 200, 50 }, "Settings");
	backTitle->SetObserver(this);
	
}

void Scene::LoadIntro()
{

	pugi::xml_document data;
	pugi::xml_parse_result result = data.load_file("save.xml");

	if (result == true)
	{
		saveExists = true;
	}
	else
	{
		saveExists = false;
	}

	if (resume != nullptr) 
	{
		if (!saveExists)
		{
			resume->state = GuiControlState::DISABLED;
		}
		else if (saveExists)
		{
			resume->state = GuiControlState::NORMAL;
		}
	}

	app->entityManager->player = nullptr;
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


void Scene::UpdateMenu(float dt)
{
	play->Update(dt);
	resume->Update(dt);
	settings->Update(dt);
	credits->Update(dt);
	quit->Update(dt);

	
}

void Scene::DrawMenu()
{
	play->Draw();
	app->render->DrawText(uiFont, playButton, 620, 302, 40, 0, { 0, 0, 0, 255 });
	resume->Draw();
	app->render->DrawText(uiFont, continueButton, 590, 372, 40, 0, { 0, 0, 0, 255 });
	settings->Draw();
	app->render->DrawText(uiFont, settingsButton, 590, 442, 40, 0, { 0, 0, 0, 255 });
	credits->Draw();
	app->render->DrawText(uiFont, creditsButton, 600, 512, 40, 0, { 0, 0, 0, 255 });
	quit->Draw();
	app->render->DrawText(uiFont, exitButton, 615, 582, 40, 0, { 0, 0, 0, 255 });
}

void Scene::UpdateSettings(float dt)
{
	back->Update(dt);
	musicVol->Update(dt);
	fxVol->Update(dt);
	fullscreen->Update(dt);
	vsync->Update(dt);
}

void Scene::DrawSettings()
{
	back->Draw();
	app->render->DrawText(uiFont, backButton, 610, 580, 40, 0, { 0, 0, 0, 255 });
	musicVol->Draw();
	app->render->DrawText(uiFont, musicVolButton, 490, 260, 40, 0, { 255, 255, 255, 255 });
	fxVol->Draw();
	app->render->DrawText(uiFont, sfxVolButton, 490, 330, 40, 0, { 255, 255, 255, 255 });
	fullscreen->Draw();
	app->render->DrawText(uiFont, fullscreenButton, 520, 440, 40, 0, { 255, 255, 255, 255 });
	vsync->Draw();
	app->render->DrawText(uiFont, vsyncButton, 520, 510, 40, 0, { 255, 255, 255, 255 });
}

void Scene::UpdatePauseSettings(float dt)
{
	backPause->Update(dt);
	musicVol->Update(dt);
	fxVol->Update(dt);
	fullscreen->Update(dt);
	vsync->Update(dt);
}

void Scene::DrawPauseSettings()
{
	backPause->Draw();
	app->render->DrawText(uiFont, backButton, 610, 580, 40, 0, { 0, 0, 0, 255 });
	musicVol->Draw();
	app->render->DrawText(uiFont, musicVolButton, 490,  260, 40, 0, { 255, 255, 255, 255 });
	fxVol->Draw();
	app->render->DrawText(uiFont, sfxVolButton, 490, 330, 40, 0, { 255, 255, 255, 255 });
	fullscreen->Draw();
	app->render->DrawText(uiFont, fullscreenButton, 520, 440, 40, 0, { 255, 255, 255, 255 });
	vsync->Draw();
	app->render->DrawText(uiFont, vsyncButton, 520, 510, 40, 0, { 255, 255, 255, 255 });
}

void Scene::UpdatePause(float dt)
{
	unpause->Update(dt);
	settingsPause->Update(dt);
	backTitle->Update(dt);
	quit->Update(dt);
	
}

void Scene::DrawPause()
{
	unpause->Draw();
	app->render->DrawText(uiFont, resumeButton, 580, 300, 40, 0, { 0, 0, 0, 255 });
	settingsPause->Draw();
	app->render->DrawText(uiFont, settingsButton, 575, 370, 40, 0, { 0, 0, 0, 255 });
	backTitle->Draw();
	app->render->DrawText(uiFont, returnButton, 565, 440, 40, 0, { 0, 0, 0, 255 });
	quit->Draw();
	app->render->DrawText(uiFont, exitButton, 615, 580, 40, 0, { 0, 0, 0, 255 });
}

void Scene::UpdateCredits(float dt)
{
	back->Update(dt);
}

void Scene::DrawCredits()
{
	app->render->DrawTexture(img, 0, 0, NULL);
	back->Draw();
	app->render->DrawText(uiFont, backButton, 615, 583, 40, 0, { 0, 0, 0, 255 });

}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) LoadLevel1();												//play
		else if (control->id == 2) app->LoadGame();										//resume
		else if (control->id == 3) { settingsActive = true; intro = false; }			//settings
		else if (control->id == 4) { creditsActive = true; intro = false; }				//credits
		else if (control->id == 5) playing = false;										//quit
		else if (control->id == 6) intro = true;										//back
		else if (control->id == 7) { pauseGame = false; printPause = true; }			//unpause
		else if (control->id == 8) { settingsPauseActive = true; printPause = false; }	//settingspause
		else if (control->id == 9) { intro = true; LoadIntro(); pauseGame = false; printPause = false;}						//backtitle
		else if (control->id == 10) { settingsPauseActive = false; printPause = true; }	//backpause
		break;
	case GuiControlType::SLIDER:
		if (control->id == 1) app->audio->ChangeMusicVolume(musicVol->ReturnValue());
		else if (control->id == 2) app->audio->ChangeFxVolume(fxVol->ReturnValue());
		break;
		break;
	case GuiControlType::CHECKBOX:

		if (control->id == 1) { app->win->fullscreen = !app->win->fullscreen; app->win->SetFullScreen(); }
		else if (control->id == 2){}
		break;

	}

	return true;
	}
}

/*bool Scene::OnGuiMouseHoverEvent(GuiControl* control)
{
	switch (control->state)
	{
	case GuiControlState::FOCUSED:
		app->audio->PlayFx(clickFx);
		break;
	}
}*/



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