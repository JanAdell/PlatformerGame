#include "Defs.h"
#include "Log.h"
#include "Player.h"
#include "Input.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Animation.h"
#include "SString.h"

#define GRAVITY 0.7f
//#define COLLIDER_OFFSET -25

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

	pugi::xml_node node = player_node.child("position");
	position.x = node.attribute("x").as_int();
	position.y = node.attribute("y").as_int();
	speed.x = 0;
	speed.y = 0;
	size.x = node.attribute("w").as_int();
	size.y = node.attribute("h").as_int();

	currentAnim = &idleAnim;

	return true;
}


bool Player::Start()
{
	characterTex = app->tex->Load("Assets/textures/player.png");

	return true;
}

bool Player::PreUpdate()
{
	currentAnim = &idleAnim;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		flip = SDL_FLIP_HORIZONTAL;
		speed.x -= moveSpeed;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		currentAnim = &runAnim;
		speed.x += moveSpeed;
	}

	if (app->input->GetKey(SDL_SCANCODE_S)== KEY_REPEAT)
	{
		currentAnim = &duckAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		flip = SDL_FLIP_NONE;
		speed.x += moveSpeed;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		speed.x -= moveSpeed;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		speed.y = jumpPower;
		solidGround = false;
		cont++;
	}
	doubleJump = true;

	if (doubleJump && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		cont++;
		if (cont == 3)
		{
			speed.y += -8;
			doubleJump = false;
			cont = 0;
		}

	}
	if (speed.y <= maxJumpHeight)
	{
		speed.y += GRAVITY;
		solidGround = false;
	}
	position.y += speed.y;

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		currentAnim = &deathAnim;
	}

	return true;
}

bool Player::Update(float dt)
{

	deltaTime = SDL_GetTicks() - lastTime;
	deltaTime /= 200;
	lastTime = SDL_GetTicks();
	acceleration.y = 0.0f;

	if (speed.y > 0)
	{
		speed.y = 0.0f;
	}

	if (speed.x != 0)
	{
		currentAnim = &runAnim;
		currentAnim = &runAnim;
		if (speed.x < 0)
		{
			flip = SDL_FLIP_HORIZONTAL;
		}
		else
		{
			flip = SDL_FLIP_NONE;
		}
	}
	if (speed.y != 0)
	{
		currentAnim = &jumpAnim;
	}
	speed.x = speed.x + acceleration.x * deltaTime;
	speed.y = speed.y + acceleration.y * deltaTime;

	position.x = position.x + speed.x * deltaTime;
	position.y = position.y + speed.y * deltaTime;

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

	return true;
}

bool Player::Load(pugi::xml_node&)
{
	return true;
}

bool Player::Save(pugi::xml_node&) const
{
	return true;
}

fPoint Player::GetPos() const
{
	return position;
}

fPoint Player::GetSpeed() const
{
	return speed;
}