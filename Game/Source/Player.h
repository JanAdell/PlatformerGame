#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Animation.h"
#include "Entity.h"
#include "Collisions.h"



//#pragma comment(lib, "SDL_mixer/libx86/SDL2_mixer.lib")

struct SDL_Texture;
struct SDL_Rect;
struct Collider;

enum PlayerState
{
	IDLE,
	RUNNING,
	JUMPING,
	FALLING,
	DEAD,
	GOD_MODE,
	MAX_STATES
};

class Player : public Entity
{
public:
	Player(const fPoint& position);
	~Player();
	// Called before render is available
	
	// Called before the first frame
	bool Start() override;
	// Called each loop iteration
	void PreUpdate(float dt) override;
	// Called each loop iteration
	void Update(float dt);
	// Called each loop iteration
	void PostUpdate(float dt);
	// Called before quitting
	void CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	bool GoRight();
	void OnCollision(Collider* c1, Collider* c2);
	void AmmoUpdate();
		
	fPoint GetPos() const;
	fPoint GetSpeed() const;

	bool firstUpdate = true;
	bool checkpoint = false;
	bool lastMove;
	float hitCd;
	
	SDL_Rect* rect = nullptr;
	SDL_Texture* hpTex = nullptr;
	SDL_Texture* ammoTex = nullptr;

	Animation lifeAnim;
	Animation ammoAnim;

private:
	float moveSpeed = 20;
			
	float deltaTime = 0;
	Uint32 lastTime = 0;
	Uint32 currTime = 0;

	PlayerState state = FALLING;
	pugi::xml_node playerNode;
	bool doubleJump;
	int cont = 0;
	bool solidGround;
	int jumpPower = -10;
};

#endif 