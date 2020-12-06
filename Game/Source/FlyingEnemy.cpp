#include "App.h"
#include "FlyingEnemy.h"
#include "Collisions.h"
#include "Render.h"
#include "Map.h"
#include "Pathfinding.h"
#include "EntityManager.h"

FlyingEnemy::FlyingEnemy(const fPoint position) : Enemy(position, "FlyingEnemy", ENTITY_TYPE::FLYING_ENEMY)
{
	name.create("FlyingEnemy");
	AwakeEntity(entNode);
}

FlyingEnemy::~FlyingEnemy()
{
}

bool FlyingEnemy::Start()
{
	characterTex = app->tex->Load("Assets/textures/octopus.png");
	collider = app->collisions->AddCollider({ (int)position.x + offset.x,(int)position.y + offset.y, size.x, size.y }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)app->entityManager);
	move_radius = 150;
	currentAnim = &enemyFly;
	return true;
}



void FlyingEnemy::Update(float dt)
{
	
		fPoint direction;
		iPoint enemy_pos = app->map->WorldToMap(position.x + offset.x, position.y + offset.y);
		if (position.DistanceManhattan(app->entityManager->player->position) <= search)
		{
			iPoint player_pos = app->map->WorldToMap(app->entityManager->player->position.x + app->entityManager->player->size.x / 2, app->entityManager->player->position.y + app->entityManager->player->size.y);
			player_pos.y -= 1;
			int a = app->pathfinding->CreatePath(enemy_pos, player_pos, ENTITY_TYPE::FLYING_ENEMY);
			if (a != -1 && app->entityManager->player)
			{
				enemy_path = app->pathfinding->GetLastPath();
 				app->pathfinding->DrawPath(enemy_path);

				if (enemy_path->Count() > 0)
				{
					fPoint next_node(enemy_path->At(0)->x, enemy_path->At(0)->y);

					direction.create(next_node.x - enemy_pos.x, next_node.y - enemy_pos.y);
					position.x += direction.x * speed.x;
					position.y += direction.y * speed.y;
				}
			}
			if (direction.x > 0)
			{
				flip = SDL_RendererFlip::SDL_FLIP_NONE;
			}
			else if (direction.x < 0)
			{
				flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
			}

		}

		else
		{
			fPoint objective;
			iPoint enemy_size_pos = app->map->WorldToMap(position.x + size.x, position.y + size.y);

			iPoint cell_right(enemy_size_pos.x + 2, enemy_size_pos.y);
			iPoint cell_left(enemy_pos.x - 2, enemy_size_pos.y);
			iPoint cell_down(enemy_size_pos.x, enemy_size_pos.y + 2);
			iPoint cell_up(enemy_pos.x, enemy_size_pos.y - 2);

			if (position.DistanceTo(initial_pos) > move_radius)
			{
				if (initial_pos.x < position.x || !app->pathfinding->IsWalkable(cell_right))
				{
					
					flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
				}
				else if (initial_pos.x > position.x || !app->pathfinding->IsWalkable(cell_left))
				{
					
					flip = SDL_RendererFlip::SDL_FLIP_NONE;
				}

				if (initial_pos.y < position.y || !app->pathfinding->IsWalkable(cell_up))
				{
					objective.create(enemy_pos.x - speed.x, enemy_pos.y);
				}
				else if (initial_pos.y > position.y || !app->pathfinding->IsWalkable(cell_down))
				{
					objective.create(enemy_pos.x + speed.x, enemy_pos.y);
				}
			}
			if (go_right)
				objective.create(enemy_pos.x + 1, enemy_pos.y);
			else
				objective.create(enemy_pos.x - 1, enemy_pos.y);


			if (objective.x != 0)
			{
				direction.create(objective.x - enemy_pos.x, objective.y - enemy_pos.y);
				position.x += direction.x * speed.x * app->dtMove;
 				position.y += direction.y * speed.y * app->dtMove;
			}
		}
		
		if(collider != nullptr)
			collider->SetPos((int)position.x + offset.x, (int)position.y + offset.y);
		
}

void FlyingEnemy::Draw()
{
	if (currentAnim != nullptr)
		app->render->DrawTexture(characterTex, (int)position.x, (int)position.y, &currentAnim->GetCurrentFrame(), 1.0F, flip);
}

void FlyingEnemy::ChangeDir()
{
	go_right = !go_right;
}

void FlyingEnemy::CleanUp()
{
	app->tex->UnLoad(characterTex);
	if (collider != nullptr)
		collider->to_delete = true;
	currentAnim = nullptr;
}
