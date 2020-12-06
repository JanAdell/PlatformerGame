#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Enemy.h"


Collisions::Collisions()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER][COLLIDER] = false;
	matrix[COLLIDER][NEXTLVL] = false;
	matrix[COLLIDER][COLLIDER_PLAYER] = true;
	matrix[COLLIDER][COLLIDER_DAMAGE] = false;
	matrix[COLLIDER][CHECKPOINT] = false;
	matrix[COLLIDER][COLLIDER_PICKUP] = false;
	matrix[COLLIDER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER][COLLIDER_BULLET] = true;

	matrix[COLLIDER_PLAYER][COLLIDER] = true;
	matrix[COLLIDER_PLAYER][NEXTLVL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DAMAGE] = true;
	matrix[COLLIDER_PLAYER][CHECKPOINT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PICKUP] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_BULLET] = false;

	matrix[NEXTLVL][COLLIDER] = false;
	matrix[NEXTLVL][NEXTLVL] = false;
	matrix[NEXTLVL][COLLIDER_PLAYER] = true;
	matrix[NEXTLVL][COLLIDER_DAMAGE] = false;
	matrix[NEXTLVL][CHECKPOINT] = false;
	matrix[NEXTLVL][COLLIDER_PICKUP] = false;
	matrix[NEXTLVL][COLLIDER_ENEMY] = false;
	matrix[NEXTLVL][COLLIDER_BULLET] = false;

	matrix[COLLIDER_DAMAGE][COLLIDER] = false;
	matrix[COLLIDER_DAMAGE][NEXTLVL] = false;
	matrix[COLLIDER_DAMAGE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DAMAGE][COLLIDER_DAMAGE] = false;
	matrix[COLLIDER_DAMAGE][CHECKPOINT] = false;
	matrix[COLLIDER_DAMAGE][COLLIDER_PICKUP] = false;
	matrix[COLLIDER_DAMAGE][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_DAMAGE][COLLIDER_BULLET] = false;

	matrix[CHECKPOINT][COLLIDER] = false;
	matrix[CHECKPOINT][NEXTLVL] = false;
	matrix[CHECKPOINT][COLLIDER_PLAYER] = true;
	matrix[CHECKPOINT][COLLIDER_DAMAGE] = false;
	matrix[CHECKPOINT][CHECKPOINT] = false;
	matrix[CHECKPOINT][COLLIDER_PICKUP] = false;
	matrix[CHECKPOINT][COLLIDER_ENEMY] = false;
	matrix[CHECKPOINT][COLLIDER_BULLET] = false;

	matrix[COLLIDER_PICKUP][COLLIDER] = false;
	matrix[COLLIDER_PICKUP][NEXTLVL] = false;
	matrix[COLLIDER_PICKUP][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PICKUP][COLLIDER_DAMAGE] = false;
	matrix[COLLIDER_PICKUP][CHECKPOINT] = false;
	matrix[COLLIDER_PICKUP][COLLIDER_PICKUP] = false;
	matrix[COLLIDER_PICKUP][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_PICKUP][COLLIDER_BULLET] = false;

	matrix[COLLIDER_ENEMY][COLLIDER] = true;
	matrix[COLLIDER_ENEMY][NEXTLVL] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_DAMAGE] = false;
	matrix[COLLIDER_ENEMY][CHECKPOINT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PICKUP] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_BULLET] = true;

	matrix[COLLIDER_BULLET][COLLIDER] = true;
	matrix[COLLIDER_BULLET][NEXTLVL] = false;
	matrix[COLLIDER_BULLET][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_BULLET][COLLIDER_DAMAGE] = false;
	matrix[COLLIDER_BULLET][CHECKPOINT] = false;
	matrix[COLLIDER_BULLET][COLLIDER_PICKUP] = false;
	matrix[COLLIDER_BULLET][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_BULLET][COLLIDER_BULLET] = false;
}


Collisions::~Collisions()
{}

bool Collisions::PreUpdate(float dt)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}


	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{

		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];


		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{

			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	return true;
}


bool Collisions::Update(float dt)
{

	// To be implemented debug capabilities for painting colliders and godmode
	DebugDraw();
	GodMode();

	return true;

}

void Collisions::DebugDraw()
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debug = !debug;
		//app->pathfinding->DrawPath();
	}
		

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER:
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case NEXTLVL:
			app->render->DrawRectangle(colliders[i]->rect, 100, 100, 100, alpha);
			break;
		case COLLIDER_PLAYER:
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_DAMAGE:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case CHECKPOINT:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 255, alpha);
			break;
		case COLLIDER_ENEMY:
			app->render->DrawRectangle(colliders[i]->rect, 200, 200, 200, alpha);
			break;
		case COLLIDER_BULLET:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 200, alpha);
			break;
		}

	}
}

bool Collisions::CleanUp()
{
	//LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	*colliders = nullptr;

	return true;

}

Collider* Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}

void Collisions::GodMode()
{
	matrix[COLLIDER_PLAYER][COLLIDER_DAMAGE] = !app->god_mode;
	matrix[COLLIDER_PLAYER][COLLIDER] = !app->god_mode;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = !app->god_mode;

	matrix[COLLIDER_DAMAGE][COLLIDER_PLAYER] = !app->god_mode;
	matrix[COLLIDER][COLLIDER_PLAYER] = !app->god_mode;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = !app->god_mode;

	
}
