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
#include "Map.h"
#include "SString.h"

#define GRAVITY 1.0f
#define COLLIDER_OFFSET -25

Player::Player() : Module()
{
	name.create("player");
	
}

bool Player::Awake(pugi::xml_node& player_node)
{
	this->player_node = player_node;
	//pugi::xml_node node = player_node.child("animation");
	//idleAnim = new Animation();
	for (pugi::xml_node node = player_node.child("animation"); node; node = node.next_sibling("animation"))
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
		}

	}

	if (app->scene->Lvl == 1)
	{
		pugi::xml_node data = player_node.child("positionLvl1");
		position.x = data.attribute("x").as_int();
		position.y = data.attribute("y").as_int();
		size.x = data.attribute("w").as_int();
		size.y = data.attribute("h").as_int();
	}

	else
	{
		pugi::xml_node data = player_node.child("positionLvl2");
		position.x = data.attribute("x").as_int();
		position.y = data.attribute("y").as_int();
		size.x = data.attribute("w").as_int();
		size.y = data.attribute("h").as_int();
	}
	
	return true;
}


bool Player::Start()
{
	characterTex = app->tex->Load("Assets/textures/player.png");
	if (app->scene->Lvl == 1) {
		pugi::xml_node node = player_node.child("positionLvl1");
		position.x = node.attribute("x").as_int();
		position.y = node.attribute("y").as_int();
		size.x = node.attribute("w").as_int();
		size.y = node.attribute("h").as_int();
		collider_player = app->collisions->AddCollider({ node.attribute("x").as_int(),node.attribute("y").as_int() ,node.attribute("w").as_int() ,node.attribute("h").as_int() }, COLLIDER_TYPE::COLLIDER_PLAYER, this);
	}
	if (app->scene->Lvl == 2) {
		pugi::xml_node node = player_node.child("positionLvl2");
		position.x = node.attribute("x").as_int();
		position.y = node.attribute("y").as_int();
		size.x = node.attribute("w").as_int();
		size.y = node.attribute("h").as_int();
		collider_player = app->collisions->AddCollider({ node.attribute("x").as_int(),node.attribute("y").as_int() ,node.attribute("w").as_int() ,node.attribute("h").as_int() }, COLLIDER_TYPE::COLLIDER_PLAYER, this);
	}
	cont = 0;

	return true;
}

bool Player::PreUpdate()
{
	currentAnim = &idleAnim;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		flip = SDL_FLIP_HORIZONTAL;
		speed.x = -moveSpeed;
		currentAnim = &runAnim;
		
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		currentAnim = &runAnim;
		speed.x = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_S)== KEY_REPEAT)
	{
		currentAnim = &duckAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		flip = SDL_FLIP_NONE;
		speed.x = moveSpeed;
		currentAnim = &runAnim;
		
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		speed.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && state != JUMPING && doubleJump == true)
	{
		speed.y = -100;
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

	

	return true;
}

void Player::OnCollision(Collider* col1, Collider* col2)
{
	
	if (col1->type == COLLIDER_TYPE::COLLIDER || col2->type == COLLIDER_TYPE::COLLIDER)
	{
		//vertical collisions
		if (collider_player->rect.x < col1->rect.x + col1->rect.w  && collider_player->rect.x + collider_player->rect.w > col1->rect.x || collider_player->rect.x < col2->rect.x + col2->rect.w  && collider_player->rect.x + collider_player->rect.w > col2->rect.x )
		{
			if (collider_player->rect.y + collider_player->rect.h > col1->rect.y && collider_player->rect.y < col1->rect.y && state != PLAYER_STATE::JUMPING && state != PLAYER_STATE::RUNNING || collider_player->rect.y + collider_player->rect.h > col2->rect.y && collider_player->rect.y < col2->rect.y && state != PLAYER_STATE::JUMPING && state != PLAYER_STATE::RUNNING)
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

			else if (collider_player->rect.y < col1->rect.y + col1->rect.h && collider_player->rect.y + collider_player->rect.h > col1->rect.y + col1->rect.h || collider_player->rect.y < col2->rect.y + col2->rect.h && collider_player->rect.y + collider_player->rect.h > col2->rect.y + col2->rect.h)
			{
				state = FALLING;
				speed.y = 0;
				
			
			}
		}

		//horitzontal collisions
		if (collider_player->rect.y < col1->rect.y + col1->rect.h - 5 && collider_player->rect.y + collider_player->rect.h > col1->rect.y + 5 || collider_player->rect.y < col2->rect.y + col2->rect.h - 5 && collider_player->rect.y + collider_player->rect.h > col2->rect.y + 5)
		{

			//LEFT
			if (collider_player->rect.x < col1->rect.x + col1->rect.w && collider_player->rect.x + collider_player->rect.w > col1->rect.x + col1->rect.w || collider_player->rect.x < col2->rect.x + col2->rect.w && collider_player->rect.x + collider_player->rect.w > col2->rect.x + col2->rect.w)
			{
				speed.x = moveSpeed;
				doubleJump = true;
			}

			//RIGHT
			else if (collider_player->rect.x + collider_player->rect.w > col1->rect.x && collider_player->rect.x < col1->rect.x || collider_player->rect.x + collider_player->rect.w > col2->rect.x && collider_player->rect.x < col2->rect.x)
			{	
				speed.x = -moveSpeed;
				doubleJump = true;
			}
		}

	}
		
			else if (col1->type == COLLIDER_TYPE::NEXTLVL || col2->type == COLLIDER_TYPE::NEXTLVL)
			{
				if(app->scene->Lvl == 1)
					app->scene->LoadLevel2();

				if (app->scene->Lvl == 2)
					app->scene->LoadLevel1();
			}
			else if (col1->type == COLLIDER_TYPE::COLLIDER_DAMAGE || col2->type == COLLIDER_TYPE::COLLIDER_DAMAGE)
			{
				state = DEAD;
			}
	
}

bool Player::Update(float dt)
{
	
	deltaTime = SDL_GetTicks() - lastTime;
	deltaTime /= 200;
	lastTime = SDL_GetTicks();
			
		
	switch (state) 
	{
		case IDLE:
			acceleration.y = 0;
			speed.y = 0;
			break;
		
		case FALLING:
			acceleration.y = GRAVITY;
			currentAnim = &jumpAnim;
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

			if (app->scene->Lvl == 1)
			{
				pugi::xml_node node = player_node.child("positionLvl1");
				position.x = node.attribute("x").as_int();
				position.y = node.attribute("y").as_int();
			}
			else if (app->scene->Lvl == 2)
			{
				pugi::xml_node node = player_node.child("positionLvl2");
				position.x = node.attribute("x").as_int();
				position.y = node.attribute("y").as_int();
			}

			break;

	}

	
	speed.y = speed.y + acceleration.y;

	if (speed.y >= 60) {
		speed.y = 60;
	}
	if (speed.y <= -50) {
		speed.y = -50;
	}
	
	position.x = position.x + speed.x * deltaTime;
	position.y = position.y + speed.y * deltaTime;

	collider_player->SetPos(position.x, position.y);
		
	
	state = FALLING;
	speed.x = 0;

	return true;
}

bool Player::PostUpdate()
{
	app->render->DrawTexture(characterTex, (int)position.x, (int)position.y, &currentAnim->GetCurrentFrame(), 1.0f, flip);

	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(characterTex);
	if(collider_player != nullptr)
		collider_player->to_delete = true;

	return true;
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