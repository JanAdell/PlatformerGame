#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Animation.h"
#include "Entity.h"

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
	bool Awake(pugi::xml_node&);
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

	void OnCollision(Collider* c1, Collider* c2);
	Collider* colliderPlayer;

	bool IsStanding();
	fPoint position;

	fPoint GetPos() const;
	fPoint GetSpeed() const;

	bool firstUpdate = true;
	//Collider* colliderPlayer = nullptr;
	fPoint spawnPos;
	fPoint checkpointPos;
	bool checkpoint = false;

private:
	float moveSpeed = 30;
	//float contpeed = -25;

	iPoint size;
	iPoint offset;
	fPoint acceleration;
			
	Animation idleAnim;
	Animation jumpAnim;
	Animation runAnim;
	Animation deathAnim;
	Animation duckAnim;

	pugi::xml_node playerNode;
	SDL_RendererFlip flip;

	float deltaTime = 0;
	Uint32 lastTime = 0;
	Uint32 currTime = 0;

	PlayerState state = FALLING;


	bool doubleJump;
	int cont = 0;
	bool solidGround;
	int jumpPower = -10;
};

#endif 