#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;
class GuiControl;
class GuiButton;
class GuiSlider;
class GuiCheckBox;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt)override;

	// Called before quitting
	bool CleanUp();

	void LoadLevel();
	void LoadLevel1();
	void LoadLevel2();
	void LoadIntro();

	void LoadGUI();

	void UpdateMenu(float dt);
	void DrawMenu();

	void UpdateSettings(float dt);
	void DrawSettings();

	void UpdateCredits(float dt);
	void DrawCredits();

	void UpdatePause(float dt);
	void DrawPause();

	void UpdatePauseSettings(float dt);
	void DrawPauseSettings();

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool Load(pugi::xml_node& data);

	bool Save(pugi::xml_node& data) const;

	bool intro = true;
	bool settingsActive = false;
	bool settingsPauseActive = false;
	bool creditsActive = false;
	bool pauseGame = false;

	uint lvl = 1;
	pugi::xml_node playerNodeScene;

private:
	SDL_Texture* img = nullptr;
	SDL_Texture* creditsBack = nullptr;

	GuiButton* play;
	GuiButton* resume;
	GuiButton* settings;
	GuiButton* credits;
	GuiButton* quit;
	GuiButton* back;
	GuiButton* backTitle;
	GuiButton* unpause;
	GuiButton* settingsPause;
	GuiButton* backPause;
	GuiSlider* musicVol;
	GuiSlider* fxVol;
	GuiCheckBox* fullscreen;
	GuiCheckBox* vsync;
	
	int squarifiedFont = -1;
	bool playing = true;
	bool printPause = true;

	Font* uiFont;
	char playButton[64] = { 0 };
	char continueButton[64] = { 0 };
	char resumeButton[64] = { 0 };
	char settingsButton[64] = { 0 };
	char creditsButton[64] = { 0 };
	char exitButton[64] = { 0 };
	char fullscreenButton[64] = { 0 };
	char vsyncButton[64] = { 0 };
	char sfxVolButton[64] = { 0 };
	char musicVolButton[64] = { 0 };
	char backButton[64] = { 0 };
	char returnButton[64] = { 0 };
};

#endif // __SCENE_H__