#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class GuiManager : public Module
{
public:

	GuiManager(bool startEnabled);

	// Destructor
	virtual ~GuiManager();

	// Called before render is available
	//bool Awake(pugi::xml_node&);

	// Called before the first frame
	//bool Start();

	bool Update(float dt);

	bool PostUpdate(float dt);

	// Called before quitting
	//bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, SDL_Rect rect, const char* text);
	void DestroyGuiControl(GuiControl* entity);
	void DestroyAllGuiControl();
	void AddGuiControl(GuiControl* entity);

public:

	List<GuiControl*> controls;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif // __GUIMANAGER_H__
