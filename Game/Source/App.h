#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PugiXml/src/pugixml.hpp"
#include "Timer.h"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Map;
//class Font;
class FadeToBlack;
class Player;
class EntityManager;
class Collisions;
class GuiManager;
class Pathfinding;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	//void GetSaveGames();

	float GetDt();
	float GetTime();
	void SetFPScap(uint fps)
	{
		if (fps > 0)
			fpsCap = 1000 / fps;
	}

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Save & Load
	bool LoadGameNow();
	bool SaveGameNow() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	Map* map;
	//Font* font;
	FadeToBlack* fade;
	EntityManager* entityManager;
	Collisions* collisions;
	Pathfinding* pathfinding;
	GuiManager* gui;
	
public:

	bool godMode = false;
	bool pause = false;
	float dtMove;
	

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	mutable bool		saveRequest;
	bool				loadRequest;
	SString			loadGame;
	mutable SString	saveGame;

	List<Module *> modules;

	// TODO 2: Create new variables from pugui namespace:
	// a xml_document to store the config file and
	// two xml_node to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	uint frames;
	float dt;
	float lastTime;

	bool				capFps = true;
	uint				framesCap = 0;
	Timer				timerFrames;
	Timer				timerFramesSecond;
	Timer				timerStarting;
	Uint64				timeLastFramesSecond;
	Uint64				timeFrameSecond = 0;
	Uint64				frameCount = 0;
	Uint32				fpsCap = 0;
};

extern App* app;

#endif	// __APP_H__