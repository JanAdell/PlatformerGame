#include "Enemy.h"
#include "Collisions.h"
#include "Map.h"

Enemy::Enemy(const fPoint position, const char* name, ENTITY_TYPE type) : Entity(position, name, type)
{
	currentAnim = &anim_idle;

	pugi::xml_document config_file;
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	ret = config_file.child("config").child("entities").child("GroundEnemy");
	search = ret.child("path").attribute("search").as_int();
	path_speed = ret.child("path").attribute("speed").as_float();
}

Enemy::~Enemy()
{

}

void Enemy::Update(float dt)
{
	
}

void Enemy::PushBack()
{
	/*for (ListItem<EntitiesAnim*>* animation = data.animations.start; animation != nullptr; animation = animation->next)
	{
		ListItem<SDL_Rect*>* frames_item;
		switch (animation->data->states)
		{
		case EntityState::IDLE:
			for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
				anim_idle.PushBack(*(frames_item)->data);
			anim_idle.speed = animation->data->speed;
			break;
		case EntityState::WALKING:
			for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
				anim_walking.PushBack(*(frames_item)->data);
			anim_walking.speed = animation->data->speed;
			break;
		case EntityState::DEAD:
			for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
				anim_dying.PushBack(*(frames_item)->data);
			anim_dying.speed = animation->data->speed;
			break;
		default:
			break;
		}
	}*/
	anim_dying.loop = false;
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
	if (col1->type == COLLIDER_TYPE::COLLIDER)
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
			position.x += speed.x * direction.x; //this needs to be multiplied by DT
		
		}
	}

	if (col1->type == COLLIDER_TYPE::COLLIDER_PLAYER)
	{
		if (collider->rect.y < col1->rect.y + col1->rect.h && collider->rect.y + collider->rect.h > col1->rect.y + col1->rect.h)
		{
			state = EnemyState::DEAD;
			collider->to_delete = true;
		}
		else
		{
			enemy_path = nullptr;
		}
	}
}