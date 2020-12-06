#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "App.h"
#include "Point.h"
#include "Textures.h"
#include "Audio.h"
#include "Animation.h"
#include "SDL_image/include/SDL_image.h"

#include "List.h"

enum class COLLIDER_DIRECTION;

enum class EntityState {
	IDLE,
	WALKING,
	JUMP,
	FALL,
	DEAD,
	UNKNOWN
};

enum class EntityType
{
	NO_ENTITY,
	PLAYER,
	PICKUP,
	FLYING_ENEMY,
	GROUND_ENEMY,
	BULLET,
};

class Entity
{
public:

	Entity(const fPoint& position, const char* name, EntityType type);
	~Entity();

	virtual bool Start() = 0;
	virtual void PreUpdate(float dt) = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw();
	virtual void CleanUp() = 0;

	virtual void OnCollision(Collider* c1, Collider* c2) = 0;
	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };

	virtual void AwakeEntity(pugi::xml_node&);

	
public:
	SString name;
	Collider* collider = nullptr;
	
	fPoint position;
	fPoint spawnPos;
	fPoint checkpointPos;
	fPoint speed;
	iPoint size;
	iPoint offset;
	fPoint acceleration;
	float gravity;
	bool checkpoint = false;
	int checkpointFx;
	int hp;
	int ammo;
	
	EntityType type = EntityType::NO_ENTITY;
	EntityState states = EntityState::UNKNOWN;
	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;
	float animSpeed;
	Animation animIdle;
	Animation* currentAnim = nullptr;
	bool toDelete = false;

	SDL_Texture* characterTex = nullptr;
	
		
	Animation idleAnim;
	Animation jumpAnim;
	Animation runAnim;
	Animation deathAnim;
	Animation duckAnim;
	Animation enemyFly;
	Animation enemyWalk;
	Animation powerUp;
	Animation bulletFly;

	pugi::xml_node entNode;
};

#endif
