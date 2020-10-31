#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "SDL/include/SDL_render.h"
//#pragma comment(lib, "SDL_mixer/libx86/SDL2_mixer.lib")

struct SDL_Texture;
struct SDL_Rect;
struct Collider;

class Player : public Module
{
public:
	Player();
	// Called before render is available
	bool Awake(pugi::xml_node&);
	// Called before the first frame
	bool Start();
	// Called each loop iteration
	bool PreUpdate();
	// Called each loop iteration
	bool Update(float dt);
	// Called each loop iteration
	bool PostUpdate();
	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	//void OnCollision(Collider* c1, Collider* c2);
	//Collider* colliderplayer = nullptr;

	bool IsStanding();
	fPoint position;

	fPoint GetPos() const;
	fPoint GetSpeed() const;

	bool firstUpdate = true;

private:
	float moveSpeed = 30;
	float contpeed = -25;

	iPoint size;

	fPoint speed;
	fPoint acceleration;


	SDL_Texture* characterTex;
	Animation* currentAnim = nullptr;
	Animation idleAnim;
	Animation jumpAnim;
	Animation runAnim;
	Animation deathAnim;

	pugi::xml_node player_node;
	SDL_RendererFlip flip;

	float deltaTime = 0;
	Uint32 lastTime = 0;
	Uint32 currTime = 0;

	bool jumping;
	float maxJumpHeight;
	bool doubleJump;
	int cont = 0;
	bool solidGround = false;
	int jumpPower = -10;
};

#endif 