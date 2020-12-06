#include "Defs.h"
#include "Log.h"
#include "Player.h"
#include "Input.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Scene.h"
#include "Animation.h"
#include "Collisions.h"
#include "Window.h"
#include "Entity.h"

#include "Map.h"
#include "SString.h"

#define GRAVITY 2.0f
//#define COLLIDER_OFFSET -25

Player::Player(const fPoint& position) : Entity(position, "player", ENTITY_TYPE::PLAYER)
{
	name.create("player");
	
	AwakeEntity(entNode);
}

Player::~Player()
{

}


bool Player::Start()
{
	characterTex = app->tex->Load("Assets/textures/player.png");
	collider = app->collisions->AddCollider({ (int)position.x, (int)position.y ,size.x ,size.y }, COLLIDER_TYPE::COLLIDER_PLAYER, (Module*)app->entityManager);
		
	cont = 0;
	
	return true;
}

void Player::PreUpdate(float dt)
{
	currentAnim = &idleAnim;
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		flip = SDL_FLIP_HORIZONTAL;
		speed.x = -moveSpeed;
		currentAnim = &runAnim;
		app->pause = false;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		currentAnim = &runAnim;
		speed.x = 0;
		app->pause = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_S)== KEY_REPEAT && !app->god_mode)
	{
		currentAnim = &duckAnim;
		app->pause = false;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		flip = SDL_FLIP_NONE;
		speed.x = moveSpeed;
		currentAnim = &runAnim;
		app->pause = false;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		speed.x = 0;
		app->pause = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->god_mode)
	{
		position.y -= 5;
		app->pause = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->god_mode)
	{
		position.y += 5;
		app->pause = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && state != JUMPING && doubleJump == true)
	{
		speed.y = -100;
		app->pause = false;
		cont++;
		state = JUMPING;
		
		
		if (cont == 2) {
			doubleJump = false;
			
			cont = 0;
		}
	}
	
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		currentAnim = &deathAnim;
	}
	

	
}

void Player::OnCollision(Collider* col1, Collider* col2)
{
	
	if (col1->type == COLLIDER_TYPE::COLLIDER || col2->type == COLLIDER_TYPE::COLLIDER)
	{
		//vertical collisions
		if (collider->rect.x < col1->rect.x + col1->rect.w  && collider->rect.x + collider->rect.w > col1->rect.x || collider->rect.x < col2->rect.x + col2->rect.w  && collider->rect.x + collider->rect.w > col2->rect.x )
		{
			if (collider->rect.y + collider->rect.h > col1->rect.y && collider->rect.y < col1->rect.y && state != PlayerState::JUMPING && state != PlayerState::RUNNING || collider->rect.y + collider->rect.h > col2->rect.y && collider->rect.y < col2->rect.y && state != PlayerState::JUMPING && state != PlayerState::RUNNING)
			{
				state = IDLE;
				speed.y = 0;
				doubleJump = true;

				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
					flip = SDL_FLIP_HORIZONTAL;
					state = RUNNING;
				}
				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
					flip = SDL_FLIP_NONE;
					state = RUNNING;
				}
				
				
			}

			else if (collider->rect.y < col1->rect.y + col1->rect.h && collider->rect.y + collider->rect.h > col1->rect.y + col1->rect.h || collider->rect.y < col2->rect.y + col2->rect.h && collider->rect.y + collider->rect.h > col2->rect.y + col2->rect.h)
			{
				state = FALLING;
				speed.y = 0;
				
			
			}
		}

		//horitzontal collisions
		if (collider->rect.y < col1->rect.y + col1->rect.h - 5 && collider->rect.y + collider->rect.h > col1->rect.y + 5 || collider->rect.y < col2->rect.y + col2->rect.h - 5 && collider->rect.y + collider->rect.h > col2->rect.y + 5)
		{

			//LEFT
			if (collider->rect.x < col1->rect.x + col1->rect.w && collider->rect.x + collider->rect.w > col1->rect.x + col1->rect.w || collider->rect.x < col2->rect.x + col2->rect.w && collider->rect.x + collider->rect.w > col2->rect.x + col2->rect.w)
			{
				speed.x = moveSpeed;
				doubleJump = true;
			}

			//RIGHT
			else if (collider->rect.x + collider->rect.w > col1->rect.x && collider->rect.x < col1->rect.x || collider->rect.x + collider->rect.w > col2->rect.x && collider->rect.x < col2->rect.x)
			{	
				speed.x = -moveSpeed;
				doubleJump = true;
			}
		}

	}
			
	if (col1->type == COLLIDER_TYPE::NEXTLVL || col2->type == COLLIDER_TYPE::NEXTLVL)
	{
		speed.x = 0;
		acceleration.x = 0;
		speed.y = 0;
		acceleration.y = 0;
		if (app->scene->lvl == 1)
		{
			checkpoint = false;
			app->scene->LoadLevel2();
		}

		else if (app->scene->lvl == 2)
		{
			checkpoint = false;
			app->scene->LoadLevel1();
		}
	}

	else if (col1->type == COLLIDER_TYPE::COLLIDER_DAMAGE || col2->type == COLLIDER_TYPE::COLLIDER_DAMAGE)
	{
		state = DEAD;
	}

	else if (col1->type == COLLIDER_TYPE::COLLIDER_ENEMY || col2->type == COLLIDER_TYPE::COLLIDER_ENEMY)
	{
		state = DEAD;
	}
	
	else if (col1->type == COLLIDER_TYPE::CHECKPOINT || col2->type == COLLIDER_TYPE::CHECKPOINT)
	{
		app->SaveGame();

		checkpointPos.x = position.x;
		checkpointPos.y = position.y;

		checkpoint = true;
	}

	else if (col1->type == COLLIDER_TYPE::COLLIDER_PICKUP || col2->type == COLLIDER_TYPE::COLLIDER_PICKUP)
	{
		
	}
	
}

void Player::Update(float dt)
{
	
	deltaTime = app->dtMove;
				
		
	switch (state) 
	{
		case IDLE:
			acceleration.y = 0;
			speed.y = 0;
			break;
		
		case FALLING:
			if (!app->god_mode)
			{
				acceleration.y = GRAVITY;
				currentAnim = &jumpAnim;
			}
			
			break;
		
		case RUNNING:
			currentAnim = &runAnim;
			acceleration.y = 0;
			speed.y = 0;
			break;

		case JUMPING:
			acceleration.y = GRAVITY;
			break;
		
		case DEAD:
						
			if (checkpoint == false)
			{
				position.x = spawnPos.x;
				position.y = spawnPos.y;
			}
			else
			{
				position.x = checkpointPos.x;
				position.y = checkpointPos.y;
			}
		
	}

	if(!app->pause)
		speed.y = speed.y + acceleration.y;

	if (speed.y >= 60) {
		speed.y = 60;
	}
	if (speed.y <= -50) {
		speed.y = -50;
	}
	
	position.x = position.x + speed.x * deltaTime;
	position.y = position.y + speed.y * deltaTime;

	collider->SetPos(position.x + 2, position.y);
	
	
	state = FALLING;
	
	speed.x = 0;

	
}

void Player::PostUpdate(float dt)
{
			
}

void Player::CleanUp()
{
	app->tex->UnLoad(characterTex);
	if(collider != nullptr)
		collider->to_delete = true;
		
}

bool Player::Load(pugi::xml_node& data)
{
	bool ret = true;

	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	LOG("%f", position.x);
	LOG("%f", position.y);

	return ret;
}

bool Player::Save(pugi::xml_node& data) const
{
	bool ret = true;

	data.append_child("position").append_attribute("x") = position.x;
	data.child("position").append_attribute("y") = position.y;

	return ret;
}

fPoint Player::GetPos() const
{
	return position;
}

fPoint Player::GetSpeed() const
{
	return speed;
}