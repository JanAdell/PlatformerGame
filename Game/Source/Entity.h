#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "App.h"
#include "Point.h"
#include "Textures.h"
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

enum class ENTITY_TYPE
{
	NO_ENTITY,
	PLAYER,
	PLATFORM,
	FLYING_ENEMY,
	GROUND_ENEMY,
};

class Entity
{
public:

	Entity(const fPoint& position, const char* name, ENTITY_TYPE type);
	~Entity();

	virtual bool Start() = 0;
	virtual void PreUpdate(float dt) = 0;
	virtual void Update(float dt)=0;
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
	fPoint speed;
	iPoint size;
	iPoint offset;
	fPoint acceleration;
	float gravity;

	ENTITY_TYPE type = ENTITY_TYPE::NO_ENTITY;
	EntityState states = EntityState::UNKNOWN;
	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;
	float anim_speed;
	Animation anim_idle;
	Animation* currentAnim = nullptr;
	bool to_delete = false;

	SDL_Texture* characterTex = nullptr;
		
	Animation idleAnim;
	Animation jumpAnim;
	Animation runAnim;
	Animation deathAnim;
	Animation duckAnim;
	Animation enemyFly;
	Animation enemyWalk;

	pugi::xml_node entNode;
};

#endif
