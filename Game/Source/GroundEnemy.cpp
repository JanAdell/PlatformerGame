#include "App.h"
#include "GroundEnemy.h"
#include "Collisions.h"
#include "Render.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "Map.h"

GroundEnemy::GroundEnemy(const fPoint position) : Enemy(position, "GroundEnemy", ENTITY_TYPE::GROUND_ENEMY)
{
	name.create("GroundEnemy");
	AwakeEntity(entNode);
}

GroundEnemy::~GroundEnemy()
{
}

bool GroundEnemy::Start()
{
	characterTex = app->tex->Load("Assets/textures/crabwalk.png");
	collider = app->collisions->AddCollider({ (int)position.x + offset.x,(int)position.y + offset.y, size.x, size.y }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)app->entityManager);
	currentAnim = &enemyWalk;
	return true;
}



void GroundEnemy::Update(float dt)
{
	
		fPoint direction;
		iPoint enemy_pos = app->map->WorldToMap(position.x + offset.x, position.y + offset.y);
		if (position.DistanceManhattan(app->entityManager->player->position) <= search)
		{
			iPoint player_pos = app->map->WorldToMap(app->entityManager->player->position.x + app->entityManager->player->size.x / 2, app->entityManager->player->position.y + app->entityManager->player->size.y);


			if (app->pathfinding->CreatePath(enemy_pos, player_pos, ENTITY_TYPE::GROUND_ENEMY) != -1 && app->entityManager->player)
			{
				enemy_path = app->pathfinding->GetLastPath();
				app->pathfinding->DrawPath(enemy_path);

				if (enemy_path->Count() > 0)
				{
					fPoint next_node(enemy_path->At(0)->x, enemy_path->At(0)->y);

					direction.create(next_node.x - enemy_pos.x, next_node.y - enemy_pos.y);
				}
			}
		}

		else
		{
			fPoint objective;
			iPoint enemy_size_pos = app->map->WorldToMap(position.x + size.x, position.y + size.y);

			iPoint cell(enemy_size_pos.x + 1, enemy_size_pos.y + 2);
			iPoint cell1(enemy_size_pos.x + 2, enemy_size_pos.y);
			iPoint cell2(enemy_pos.x - 1, enemy_size_pos.y + 2);
			iPoint cell3(enemy_pos.x - 2, enemy_size_pos.y);

			//go right
			if (app->pathfinding->IsWalkable(cell) || !app->pathfinding->IsWalkable(cell1))
				go_right = false;
			//go left
			else if (app->pathfinding->IsWalkable(cell2) || !app->pathfinding->IsWalkable(cell3))
				go_right = true;

			if (go_right)
				objective.create(enemy_pos.x + path_speed, enemy_pos.y);
			else
				objective.create(enemy_pos.x - path_speed, enemy_pos.y);


			if (objective.x != 0)
			{
				direction.create(objective.x - enemy_pos.x, objective.y - enemy_pos.y);

			}
		}


		position.y += gravity * dt;
		position.x += direction.x * speed.x * dt;

		if (direction.x > 0)
		{
			currentAnim = &anim_walking;
			flip = SDL_RendererFlip::SDL_FLIP_NONE;
		}
		else if (direction.x < 0)
		{
			currentAnim = &anim_walking;
			flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		}
		else
		{
			currentAnim = &anim_idle;
		}
		collider->SetPos((int)position.x + offset.x, (int)position.y + offset.y);
	
	/*else if (state == EnemyState::DEAD)
	{
		currentAnim->speed = 0.0f;
		to_delete = true;
	}*/
}

void GroundEnemy::Draw()
{
	if (currentAnim != nullptr)
		app->render->DrawTexture(characterTex, (int)position.x, (int)position.y, &currentAnim->GetCurrentFrame(), 1.0f, flip);
}

void GroundEnemy::CleanUp()
{
	app->tex->UnLoad(characterTex);
	if (collider != nullptr)
		collider->to_delete = true;
	currentAnim = nullptr;
}
