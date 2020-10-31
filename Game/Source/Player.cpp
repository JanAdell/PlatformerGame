#include "Player.h"
#include "Input.h"
#include "App.h"
#include "Render.h"

bool Player::Awake(pugi::xml_node&)
{
	return true;
}

bool Player::Start()
{
	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	/*if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;
		*/
	return true;
}

void Player::OnCollision(Collider* collider)
{


}

bool Player::PostUpdate()
{
	return true;
}

bool Player::CleanUp()
{
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