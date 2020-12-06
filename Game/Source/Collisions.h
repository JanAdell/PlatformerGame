#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#define MAX_COLLIDERS 300

#include "Module.h"


enum ColliderType
{
	COLLIDER_NONE = -1,
	COLLIDER,
	COLLIDER_PLAYER,
	NEXTLVL,
	COLLIDER_DAMAGE,
	CHECKPOINT,
	COLLIDER_ENEMY,
	COLLIDER_PICKUP,
	COLLIDER_BULLET,
	COLLIDER_MAX	
};

struct Collider
{
	SDL_Rect rect;
	bool toDelete = false;
	ColliderType type;
	Module* callback = nullptr;
	float damage = 1.0;

	Collider(SDL_Rect rectangle, ColliderType type, Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};


class Collisions : public Module
{
public:

	Collisions();
	~Collisions();

	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool CleanUp() override;

	Collider* AddCollider(SDL_Rect rect, ColliderType type, Module* callback = nullptr);
	void DebugDraw();
	Collider* colliders[MAX_COLLIDERS];
	bool debug = false;

	void GodMode();
	

private:

	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	

};

#endif
