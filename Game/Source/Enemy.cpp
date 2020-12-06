#include "Enemy.h"
#include "Collisions.h"
#include "Map.h"
#include "FlyingEnemy.h"
#include "GroundEnemy.h"

Enemy::Enemy(const fPoint position, const char* name, EntityType type) : Entity(position, name, type)
{
	currentAnim = &idleAnim;

	pugi::xml_document config_file;
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	ret = config_file.child("config").child("entities").child("GroundEnemy");
	search = 300;
	pathSpeed = ret.child("path").attribute("speed").as_float();

	enemyDeath = app->audio->LoadFx("Assets/audio/fx/enemy_death.ogg");
}

Enemy::~Enemy()
{

}

void Enemy::Update(float dt)
{
	
}


bool Enemy::Load(pugi::xml_node& node)
{

	return true;
}

bool Enemy::Save(pugi::xml_node& node) const
{
	pugi::xml_node enemy_stats = node.append_child("enemy_stats");
	enemy_stats.append_attribute("type") = (int)type;
	enemy_stats.append_attribute("gravity") = (float)gravity;
	enemy_stats.append_attribute("position_x") = (int)position.x;
	enemy_stats.append_attribute("position_y") = (int)position.y;
	return true;
}


void Enemy::OnCollision(Collider* col1, Collider* col2)
{
	if (col1->type == ColliderType::COLLIDER || col2->type == ColliderType::COLLIDER)
	{
		iPoint enemy_pos = app->map->WorldToMap(position.x, position.y);
		iPoint objective = app->map->WorldToMap(col1->rect.x, col1->rect.y);
		fPoint direction(enemy_pos.x - objective.x, enemy_pos.y - objective.y);

		//vertical collisions
		if (collider->rect.x < col1->rect.x + col1->rect.w - 5 && collider->rect.x + collider->rect.w > col1->rect.x + 5)
		{
			position.y -= gravity; //this needs to be multiplied by DT

		}

		//horitzontal collisions
		if (collider->rect.y < col1->rect.y + col1->rect.h - 5 && collider->rect.y + collider->rect.h > col1->rect.y + 5)
		{
			if (type == EntityType::FLYING_ENEMY) {
				dynamic_cast<FlyingEnemy*>(this)->ChangeDir();
			}
			else if (type == EntityType::GROUND_ENEMY) {
				dynamic_cast<GroundEnemy*>(this)->ChangeDir();
			}
			position.x += speed.x * direction.x; //this needs to be multiplied by DT
		}
	}

	if (col1->type == ColliderType::COLLIDER_PLAYER || col2->type == ColliderType::COLLIDER_PLAYER)
	{
		if (collider->rect.y < col1->rect.y + col1->rect.h && collider->rect.y + collider->rect.h > col1->rect.y + col1->rect.h)
		{
			app->audio->PlayFx(enemyDeath);
			toDelete = true;
		}
		else
		{
			enemyPath = nullptr;
		}
	}
}