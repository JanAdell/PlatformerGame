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

#define GRAVITY 1.0f
//#define COLLIDER_OFFSET -25

Player::Player(const fPoint& position) : Entity(position, "player", ENTITY_TYPE::PLAYER)
{
	name.create("player");
	
}

Player::~Player()
{

}

bool Player::Awake(pugi::xml_node& playerNode)
{
	this->playerNode = playerNode;
	//pugi::xml_node node = playerNode.child("animation");
	//idleAnim = new Animation();
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
		}

	}

	if (app->scene->lvl == 1)
	{
		pugi::xml_node data = playerNode.child("positionLvl1");
		position.x = data.attribute("x").as_int();
		position.y = data.attribute("y").as_int();
		size.x = data.attribute("w").as_int();
		size.y = data.attribute("h").as_int();
	}

	else
	{
		pugi::xml_node data = playerNode.child("positionLvl2");
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
	if (app->scene->lvl == 1) {
		pugi::xml_node node = playerNode.child("positionLvl1");
		position.x = spawnPos.x;
		position.y = spawnPos.y;
		size.x = node.attribute("w").as_int();
		size.y = node.attribute("h").as_int();
		colliderPlayer = app->collisions->AddCollider({ node.attribute("x").as_int(),node.attribute("y").as_int() ,node.attribute("w").as_int() ,node.attribute("h").as_int() }, COLLIDER_TYPE::COLLIDER_PLAYER, (Module*)app->entityManager);
	}
	if (app->scene->lvl == 2) {
		pugi::xml_node node = playerNode.child("positionLvl2");
		position.x = spawnPos.x;
		position.y = spawnPos.y;
		size.x = node.attribute("w").as_int();
		size.y = node.attribute("h").as_int();
		colliderPlayer = app->collisions->AddCollider({ node.attribute("x").as_int(),node.attribute("y").as_int() ,node.attribute("w").as_int() ,node.attribute("h").as_int() }, COLLIDER_TYPE::COLLIDER_PLAYER, (Module*)app->entityManager);
	}
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
		
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		currentAnim = &runAnim;
		speed.x = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_S)== KEY_REPEAT && !app->god_mode)
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

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->god_mode)
	{
		position.y -= 5;
		
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->god_mode)
	{
		position.y += 5;
		
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
	

	
}

void Player::OnCollision(Collider* col1, Collider* col2)
{
	
	if (col1->type == COLLIDER_TYPE::COLLIDER || col2->type == COLLIDER_TYPE::COLLIDER)
	{
		//vertical collisions
		if (colliderPlayer->rect.x < col1->rect.x + col1->rect.w  && colliderPlayer->rect.x + colliderPlayer->rect.w > col1->rect.x || colliderPlayer->rect.x < col2->rect.x + col2->rect.w  && colliderPlayer->rect.x + colliderPlayer->rect.w > col2->rect.x )
		{
			if (colliderPlayer->rect.y + colliderPlayer->rect.h > col1->rect.y && colliderPlayer->rect.y < col1->rect.y && state != PlayerState::JUMPING && state != PlayerState::RUNNING || colliderPlayer->rect.y + colliderPlayer->rect.h > col2->rect.y && colliderPlayer->rect.y < col2->rect.y && state != PlayerState::JUMPING && state != PlayerState::RUNNING)
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

			else if (colliderPlayer->rect.y < col1->rect.y + col1->rect.h && colliderPlayer->rect.y + colliderPlayer->rect.h > col1->rect.y + col1->rect.h || colliderPlayer->rect.y < col2->rect.y + col2->rect.h && colliderPlayer->rect.y + colliderPlayer->rect.h > col2->rect.y + col2->rect.h)
			{
				state = FALLING;
				speed.y = 0;
				
			
			}
		}

		//horitzontal collisions
		if (colliderPlayer->rect.y < col1->rect.y + col1->rect.h - 5 && colliderPlayer->rect.y + colliderPlayer->rect.h > col1->rect.y + 5 || colliderPlayer->rect.y < col2->rect.y + col2->rect.h - 5 && colliderPlayer->rect.y + colliderPlayer->rect.h > col2->rect.y + 5)
		{

			//LEFT
			if (colliderPlayer->rect.x < col1->rect.x + col1->rect.w && colliderPlayer->rect.x + colliderPlayer->rect.w > col1->rect.x + col1->rect.w || colliderPlayer->rect.x < col2->rect.x + col2->rect.w && colliderPlayer->rect.x + colliderPlayer->rect.w > col2->rect.x + col2->rect.w)
			{
				speed.x = moveSpeed;
				doubleJump = true;
			}

			//RIGHT
			else if (colliderPlayer->rect.x + colliderPlayer->rect.w > col1->rect.x && colliderPlayer->rect.x < col1->rect.x || colliderPlayer->rect.x + colliderPlayer->rect.w > col2->rect.x && colliderPlayer->rect.x < col2->rect.x)
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
			app->scene->LoadLevel2();

		else if (app->scene->lvl == 2)
			app->scene->LoadLevel1();
	}

	else if (col1->type == COLLIDER_TYPE::COLLIDER_DAMAGE || col2->type == COLLIDER_TYPE::COLLIDER_DAMAGE)
	{
		state = DEAD;
	}
	
	else if (col1->type == COLLIDER_TYPE::CHECKPOINT || col2->type == COLLIDER_TYPE::CHECKPOINT)
	{
		app->SaveGame();
		checkpoint;
	}
	
}

void Player::Update(float dt)
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
						
			position.x = spawnPos.x;
			position.y = spawnPos.y;				
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

	colliderPlayer->SetPos(position.x + 2, position.y);
	
	
	state = FALLING;
	
	speed.x = 0;

	
}

void Player::PostUpdate(float dt)
{
	app->render->DrawTexture(characterTex, (int)position.x, (int)position.y, &currentAnim->GetCurrentFrame(), 1.0f, flip);
		
}

void Player::CleanUp()
{
	app->tex->UnLoad(characterTex);
	if(colliderPlayer != nullptr)
		colliderPlayer->to_delete = true;

	
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