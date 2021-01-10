#include "EntityManager.h"
#include "App.h"
#include "Collisions.h"
#include "Render.h"
#include "Player.h"
#include "FlyingEnemy.h"
#include "GroundEnemy.h"
#include "Pickup.h"
#include "Bullet.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.create("entities");
}

EntityManager::~EntityManager()
{
}

bool EntityManager::Awake(pugi::xml_node& info)
{
	bool ret = true;

	pugi::xml_node node = info.child("entity");
	
	return ret;
}

bool EntityManager::Start()
{
	for (ListItem<Entity*>* entityItem = entities.start; entityItem != nullptr; entityItem = entityItem->next)
	{
		entityItem->data->Start();
	}
	return true;
}

bool EntityManager::PreUpdate(float dt)
{
	bool ret = true;
	ListItem<Entity*>* entityItem = entities.start;
	while (entityItem != nullptr)
	{
		if (entityItem->data->toDelete == true)
		{
			entityItem->data->CleanUp();
			entities.del(entityItem);
		}
		else
			entityItem->data->PreUpdate(dt);

		entityItem = entityItem->next;

	}

	return ret;
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	for (ListItem<Entity*>* entityItem = entities.start; entityItem != nullptr; entityItem = entityItem->next)
	{
		entityItem->data->Update(dt);
	}
	return ret;
}

bool EntityManager::PostUpdate(float dt)
{
	bool ret = true;
	for (ListItem<Entity*>* entityItem = entities.start; entityItem != nullptr; entityItem = entityItem->next) {
		if (entityItem != nullptr) {
			entityItem->data->Draw();
		}
	}
	return ret;
}

bool EntityManager::CleanUp()
{
	ListItem<Entity*>* entityItem = entities.start;
	while (entityItem != NULL)
	{
		entityItem->data->CleanUp();
		delete entityItem->data;
		entityItem->data = nullptr;
		entityItem = entityItem->next;
	}
	entities.clear();
	/*delete player;
	player = nullptr;*/
	return true;
}

Entity* EntityManager::CreateEntity(const fPoint& position, EntityType type)
{
	Entity* entity = nullptr;
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player(position);
		break;
	case EntityType::FLYING_ENEMY:
		entity = new FlyingEnemy(position);
		break;
	case EntityType::GROUND_ENEMY:
		entity = new GroundEnemy(position);
		break;
	case EntityType::PICKUP:
		entity = new Pickup(position);
		break;
	case EntityType::BULLET:
		entity = new Bullet(position);
		break;
	default:
		break;
	
	}
	if (entity != nullptr)
		entities.add(entity);
	return entity;
}

void EntityManager::DeleteEntity(Entity* entity)
{
}

void EntityManager::OnCollision(Collider* col1, Collider* col2)
{
	for (ListItem<Entity*>* entityItem = entities.start; entityItem != nullptr; entityItem = entityItem->next)
	{
		if (entityItem != nullptr && entityItem->data != nullptr && entityItem->data->collider == col1)
		{
			entityItem->data->OnCollision(col1, col2);
		}
	}
}

bool EntityManager::Load(pugi::xml_node& file)
{
	bool ret = true;

	ListItem<Entity*>* item = entities.start;
	while (item != nullptr)
	{
		if (item->data == player)
			item->data->Load(file);
		else
			item->data->toDelete = true;
		item = item->next;
	}
	for (pugi::xml_node enemy_stats = file.child("enemy_stats"); enemy_stats != nullptr; enemy_stats = enemy_stats.next_sibling("enemy_stats"))
	{
		Entity* ent = CreateEntity(fPoint(enemy_stats.attribute("position_x").as_float(), enemy_stats.attribute("position_y").as_float()), EntityType(enemy_stats.attribute("type").as_int()));
		ent->Start();
	}
	for (pugi::xml_node pickup_stats = file.child("pickup_stats"); pickup_stats != nullptr; pickup_stats = pickup_stats.next_sibling("pickup_stats"))
	{
		Entity* ent = CreateEntity(fPoint(pickup_stats.attribute("position_x").as_float(), pickup_stats.attribute("position_y").as_float()), EntityType(pickup_stats.attribute("type").as_int()));
		ent->Start();
	}
	return ret;
}

bool EntityManager::Save(pugi::xml_node& file) const
{
	bool ret = true;
	ListItem<Entity*>* item = entities.start;
	for (; item != nullptr; item = item->next)
	{
		if (item != nullptr)
			item->data->Save(file);
	}

	return ret;
}