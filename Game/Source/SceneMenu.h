/*#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__

#include "Module.h"
//#include "Animation.h"

struct SDL_Texture;
class GuiControl;
class GuiButton;

class SceneMenu : public Module
{
public:
	SceneMenu();
	virtual ~SceneMenu();
	
	bool Awake();
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt)override;
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	bool active = true;
	SString name;
	GuiButton* play;
	GuiButton* resume;
	GuiButton* settings;
	GuiButton* credits;
	GuiButton* quit;

	SDL_Texture* introText = nullptr;

};




#endif // !__SCENE_MENU_H__*/
