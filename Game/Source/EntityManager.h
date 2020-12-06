#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Point.h"
#include "List.h"
#include "DynArray.h"

enum class EntityType;

struct EntityData
{
	EntityType type = EntityType::NO_ENTITY;
	
};

class EntityManager : public Module
{
public:

	EntityManager();
	~EntityManager();
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt) override;
	bool PostUpdate(float dt);
	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;

public:

	Entity* CreateEntity(const fPoint& position, EntityType type);
	void DeleteEntity(Entity* entity);

	void OnCollision(Collider*, Collider*);

private:

	List<Entity*> entities;

public:
	EntityData queue[15];
	Entity* player = nullptr;
	Entity* flyingEnemy = nullptr;
	Entity* groundEnemy = nullptr;
	Entity* pickUp = nullptr;
	Entity* bullet = nullptr;
};


#endif
