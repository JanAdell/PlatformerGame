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
#include "Audio.h"
#include "Entity.h"
#include "EntityManager.h"
#include "GuiManager.h"

#include "Map.h"
#include "SString.h"

#define GRAVITY 2.0f
//#define COLLIDER_OFFSET -25

Player::Player(const fPoint& position) : Entity(position, "player", EntityType::PLAYER)
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
	checkpointFx = app->audio->LoadFx("Assets/audio/fx/checkpoint.ogg");
	collider = app->collisions->AddCollider({ (int)position.x, (int)position.y ,size.x ,size.y }, ColliderType::COLLIDER_PLAYER, (Module*)app->entityManager);
	cont = 0;
	hp = 3;
	ammo = 10;
	

	for (int i = 0; i < 3; i++) {
		lifeImage = dynamic_cast<GuiImage*>(app->gui->CreateGuiControl(GuiControlType::IMAGE, 80, { 150 + 50*i, 200, 20, 20 }, "LIFE"));
	}
	for (int i = 0; i < 10; i++) {
		ammoImage = dynamic_cast<GuiImage*>(app->gui->CreateGuiControl(GuiControlType::IMAGE, 80, { 150 + 20 * i, 230, 10, 20 }, "AMMO"));
	}


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
		lastMove = false;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		currentAnim = &runAnim;
		speed.x = 0;
		app->pause = false;
			
	}

	if (app->input->GetKey(SDL_SCANCODE_S)== KEY_REPEAT && !app->godMode)
	{
		currentAnim = &duckAnim;
		app->pause = false;
		
	}
	
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		flip = SDL_FLIP_NONE;
		speed.x = moveSpeed;
		currentAnim = &runAnim;
		app->pause = false;
		lastMove = true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		speed.x = 0;
		app->pause = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->godMode)
	{
		position.y -= 5;
		app->pause = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->godMode)
	{
		position.y += 5;
		app->pause = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && state != JUMPING && doubleJump == true)
	{
		speed.y = -40;
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
		hp--;
		if (hp==0)
		{
			currentAnim = &deathAnim;
			state = DEAD;
			hp = 3;
		}
		LOG("hp: %i", hp);
	}

	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN && ammo > 0)
	{
		app->entityManager->CreateEntity({ position.x, position.y + 10 }, EntityType::BULLET);
		ammo--;
	}
	
	
}

void Player::OnCollision(Collider* col1, Collider* col2)
{
	
	if (col1->type == ColliderType::COLLIDER || col2->type == ColliderType::COLLIDER)
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
			
	if (col1->type == ColliderType::NEXTLVL || col2->type == ColliderType::NEXTLVL)
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
	

	else if (col1->type == ColliderType::COLLIDER_DAMAGE || col2->type == ColliderType::COLLIDER_DAMAGE)
	{
		state = DEAD;
		hp = 3;
	}

	else if (col1->type == ColliderType::COLLIDER_ENEMY || col2->type == ColliderType::COLLIDER_ENEMY)
	{
		if (hitCd < app->GetTime())
		{
			hp--;
			hitCd = app->GetTime() + 500;
		}

		if (hp == 0)
		{
			state = DEAD;
			hp = 3;
		}
	}
	
	else if (col1->type == ColliderType::CHECKPOINT || col2->type == ColliderType::CHECKPOINT)
	{
		if (checkpoint == false)
		{
			app->audio->PlayFx(checkpointFx);
			app->SaveGame();
			checkpointPos.x = position.x;
			checkpointPos.y = position.y;
			checkpoint = true;
		}
		else
		{
			app->SaveGame();
		}

		//checkpoint = true;
	}

	else if (col1->type == ColliderType::COLLIDER_PICKUP || col2->type == ColliderType::COLLIDER_PICKUP)
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
			if (!app->godMode)
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
		collider->toDelete = true;
		
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

bool Player::GoRight()
{
	return lastMove;
}

fPoint Player::GetPos() const
{
	return position;
}

fPoint Player::GetSpeed() const
{
	return speed;
}