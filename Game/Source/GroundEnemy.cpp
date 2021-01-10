#include "App.h"
#include "GroundEnemy.h"
#include "Collisions.h"
#include "Render.h"
#include "Scene.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "Map.h"

GroundEnemy::GroundEnemy(const fPoint position) : Enemy(position, "GroundEnemy", EntityType::GROUND_ENEMY)
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
	collider = app->collisions->AddCollider({ (int)position.x + offset.x,(int)position.y + offset.y, size.x, size.y }, ColliderType::COLLIDER_ENEMY, (Module*)app->entityManager);
	currentAnim = &enemyWalk;
	return true;
}



void GroundEnemy::Update(float dt)
{

	fPoint direction;
	iPoint enemy_pos = app->map->WorldToMap(position.x + offset.x, position.y + offset.y);
	//enemy_pos.y -= 1;
	if (app->scene->pauseGame == false)
	{
		if (position.DistanceManhattan(app->entityManager->player->position) <= search)
		{
			iPoint player_pos = app->map->WorldToMap(app->entityManager->player->position.x + app->entityManager->player->size.x / 2, app->entityManager->player->position.y + app->entityManager->player->size.y);
			player_pos.y -= 1;

			if (app->pathfinding->CreatePath(enemy_pos, player_pos, EntityType::GROUND_ENEMY) != -1 && app->entityManager->player)
			{
				enemyPath = app->pathfinding->GetLastPath();

				if (app->collisions->debug)
					app->pathfinding->DrawPath(enemyPath);

				if (enemyPath->Count() > 0)
				{
					fPoint next_node(enemyPath->At(0)->x, enemyPath->At(0)->y);

					direction.create(next_node.x - enemy_pos.x, next_node.y - enemy_pos.y);
					position.x += direction.x * speed.x;

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
				ChangeDir();
			//go left
			else if (app->pathfinding->IsWalkable(cell2) || !app->pathfinding->IsWalkable(cell3))
				ChangeDir();

			if (goRight)
				objective.create(enemy_pos.x + speed.x, enemy_pos.y);
			else
				objective.create(enemy_pos.x - speed.x, enemy_pos.y);


			if (objective.x != 0)
			{
				direction.create(objective.x - enemy_pos.x, objective.y - enemy_pos.y);
				position.y += gravity * app->dtMove;
				position.x += direction.x * speed.x * app->dtMove;
			}
		}



		if (direction.x > 0)
		{
			flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;

		}
		else if (direction.x < 0)
		{
			flip = SDL_RendererFlip::SDL_FLIP_NONE;
		}
		collider->SetPos((int)position.x + offset.x, (int)position.y + offset.y);

	}
}

void GroundEnemy::Draw()
{
	if (currentAnim != nullptr)
		app->render->DrawTexture(characterTex, (int)position.x, (int)position.y, &currentAnim->GetCurrentFrame(), 1.0F, flip);
}

void GroundEnemy::ChangeDir()
{
	if (app->scene->pauseGame == false) goRight = !goRight;
}

void GroundEnemy::CleanUp()
{
	app->tex->UnLoad(characterTex);
	if (collider != nullptr)
		collider->toDelete = true;
	currentAnim = nullptr;
}
