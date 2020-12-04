#ifndef _ENEMY_H
#define _ENEMY_H

#include "Entity.h"
#include "Pathfinding.h"
#include "Point.h"

enum class EnemyState {
	IDLE,
	WALKING,
	HIT,
	SAW,
	DEAD,
	NONE,
};

class Enemy :public Entity
{
public:
	Enemy(const fPoint position, const char* name, ENTITY_TYPE type);
	~Enemy();

public:
	bool Update(float dt);

public:
	//void LoadProperties(pugi::xml_node&);
	void PushBack();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnCollision(Collider*) override;

public:
	EnemyState state = EnemyState::IDLE;
	Animation anim_idle;
	Animation anim_walking;
	Animation anim_dying;

	int search;
	float path_speed;

	const DynArray<iPoint>* enemy_path;


}; 

#endif 
