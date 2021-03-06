#include "Entity.h"
#include "Log.h"
#include "Render.h"
#include "Scene.h"

Entity::Entity(const fPoint& position, const char* name, EntityType type) :position(position), name(name), type(type)
{
	this->position = position;
	pugi::xml_document config_file;
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config").child("entities");
	
	

	switch (type)
	{
	case EntityType::NO_ENTITY:
		break;
	case EntityType::PLAYER:
		entNode = ret.child("player");

		break;
	case EntityType::FLYING_ENEMY:
		entNode = ret.child("FlyingEnemy");

		break;
	case EntityType::GROUND_ENEMY:
		entNode = ret.child("GroundEnemy");
		
		break;
	case EntityType::PICKUP:
		entNode = ret.child("Pick");

		break;
	case EntityType::BULLET:
		entNode = ret.child("Bullet");

		break;
	default:
		break;
	}

	

}

Entity::~Entity()
{

}


void Entity::Draw()
{
	if (currentAnim != nullptr)
		app->render->DrawTexture(characterTex, position.x, position.y, &currentAnim->GetCurrentFrame(), 1.0f, flip);
	else
		app->render->DrawTexture(characterTex, position.x, position.y);
	
}



void Entity::AwakeEntity(pugi::xml_node& playerNode)
{
	size.x = playerNode.child("positionLvl1").attribute("w").as_int();
	size.y = playerNode.child("positionLvl1").attribute("h").as_int();
	
	speed.x = playerNode.child("speed").attribute("x").as_int();
	speed.y = playerNode.child("speed").attribute("y").as_int();

	if (app->scene->lvl == 1) {
		pugi::xml_node node = playerNode.child("positionLvl1");
		spawnPos.x = playerNode.child("positionLvl1").attribute("x").as_int();
		spawnPos.y = playerNode.child("positionLvl1").attribute("y").as_int();

	}
	if (app->scene->lvl == 2) {
		pugi::xml_node node = playerNode.child("positionLvl2");
		spawnPos.x = playerNode.child("positionLvl2").attribute("x").as_int();
		spawnPos.y = playerNode.child("positionLvl2").attribute("y").as_int();

	}

	for (pugi::xml_node node = playerNode.child("animation"); node; node = node.next_sibling("animation"))
	{

		for (pugi::xml_node node_iterator = node.child("frame"); node_iterator; node_iterator = node_iterator.next_sibling("frame")) {
			SDL_Rect frame;
			frame.x = node_iterator.attribute("x").as_int();
			frame.y = node_iterator.attribute("y").as_int();
			frame.w = node_iterator.attribute("w").as_int();
			frame.h = node_iterator.attribute("h").as_int();

			SString name = node.attribute("name").as_string();
			if (name == "idle")
			{
				idleAnim.PushBack(frame);
				idleAnim.speed = node.attribute("speed").as_float();
				idleAnim.loop = node.attribute("loop").as_bool();
			}
			else if (name == "jump")
			{
				jumpAnim.PushBack(frame);
				jumpAnim.speed = node.attribute("speed").as_float();
				jumpAnim.loop = node.attribute("loop").as_bool();
			}
			else if (name == "run")
			{
				runAnim.PushBack(frame);
				runAnim.speed = node.attribute("speed").as_float();
				runAnim.loop = node.attribute("loop").as_bool();
			}
			else if (name == "duck")
			{
				duckAnim.PushBack(frame);
				duckAnim.speed = node.attribute("speed").as_float();
				duckAnim.loop = node.attribute("loop").as_bool();
			}
			else if (name == "hurt")
			{
				deathAnim.PushBack(frame);
				deathAnim.speed = node.attribute("speed").as_float();
				deathAnim.loop = node.attribute("loop").as_bool();
			}
			else if (name == "fly")
			{
				enemyFly.PushBack(frame);
				enemyFly.speed = node.attribute("speed").as_float();
				enemyFly.loop = node.attribute("loop").as_bool();
			}
			else if (name == "walk")
			{
				enemyWalk.PushBack(frame);
				enemyWalk.speed = node.attribute("speed").as_float();
				enemyWalk.loop = node.attribute("loop").as_bool();
			}
			else if (name == "shot")
			{
				bulletFly.PushBack(frame);
				bulletFly.speed = node.attribute("speed").as_float();
				bulletFly.loop = node.attribute("loop").as_bool();
			}
			else if (name == "power")
			{
				powerUp.PushBack(frame);
				powerUp.speed = node.attribute("speed").as_float();
				powerUp.loop = node.attribute("loop").as_bool();
			}
		}

	}
	
}



