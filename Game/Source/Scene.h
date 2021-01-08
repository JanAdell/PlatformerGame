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
	void LoadSettings();

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool Load(pugi::xml_node& data);

	bool Save(pugi::xml_node& data) const;

	bool intro = true;
	bool settingsActive = false;
	bool creditsActive = false;

	uint lvl = 1;
	pugi::xml_node playerNodeScene;

private:
	SDL_Texture* img = nullptr;

	GuiButton* play;
	GuiButton* resume;
	GuiButton* settings;
	GuiButton* credits;
	GuiButton* quit;
	GuiButton* back;
	GuiSlider* musicVol;
	GuiSlider* fxVol;
	GuiCheckBox* fullscreen;
	GuiCheckBox* vsync;
	
	int bebasFont = -1;
};

#endif // __SCENE_H__