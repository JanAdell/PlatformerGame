#include "Pickup.h"
#include "Render.h"
#include "Collisions.h"
#include "EntityManager.h"
#include "Player.h"



Pickup::Pickup(const fPoint position) : Entity(position, "Pickup", EntityType::PICKUP)
{
	name.create("Pickup");
	AwakeEntity(entNode);
}

Pickup::~Pickup()
{

}

bool Pickup::Start()
{
	characterTex = app->tex->Load("Assets/textures/powerup.png");
	collider = app->collisions->AddCollider({ (int)position.x + offset.x,(int)position.y + offset.y, size.x, size.y }, ColliderType::COLLIDER_PICKUP, (Module*)app->entityManager);
	currentAnim = &powerUp;
	return true;
}

void Pickup::Update(float dt)
{

}

void Pickup::Draw()
{
	if (currentAnim != nullptr)
		app->render->DrawTexture(characterTex, (int)position.x, (int)position.y, &currentAnim->GetCurrentFrame(), 1.0F, flip);
}

void Pickup::CleanUp()
{
	app->tex->UnLoad(characterTex);
	if (collider != nullptr)
		collider->toDelete = true;
	currentAnim = nullptr;
}

void Pickup::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == ColliderType::COLLIDER_PLAYER || c2->type == ColliderType::COLLIDER_PLAYER)
	{
		
		
		if(app->entityManager->player->ammo <= 5) 
			app->entityManager->player->ammo += 5;
		else
			app->entityManager->player->ammo = 10;
		
		if(app->entityManager->player->hp < 3)
			app->entityManager->player->hp += 1;

		app->entityManager->player->pickupCount--;
		toDelete = true;
		
		
	}
}
