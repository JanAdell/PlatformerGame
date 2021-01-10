#include "Bullet.h"
#include "Render.h"
#include "Collisions.h"
#include "EntityManager.h"
#include "Player.h"

Bullet::Bullet(const fPoint position) : Entity(position, "Bullet", EntityType::BULLET)
{
	name.create("Bullet");
	AwakeEntity(entNode);
	characterTex = app->tex->Load("Assets/textures/shot.png");
	collider = app->collisions->AddCollider({ (int)position.x + offset.x,(int)position.y + offset.y, size.x, size.y + 10 }, ColliderType::COLLIDER_BULLET, (Module*)app->entityManager);
	goRight = dynamic_cast<Player*>(app->entityManager->player)->GoRight();
	currentAnim = &bulletFly;
}

Bullet::~Bullet()
{
}


bool Bullet::Start()
{
	
	return true;
}

void Bullet::Update(float dt)
{
	if (goRight)
		position.x += speed.x;
	else if (!goRight)
		position.x -= speed.x;

	collider->SetPos(position.x, position.y);
}

void Bullet::Draw()
{
	if (currentAnim != nullptr)
		app->render->DrawTexture(characterTex, (int)position.x, (int)position.y, &currentAnim->GetCurrentFrame(), 1.0F, flip);
}

void Bullet::CleanUp()
{
	app->tex->UnLoad(characterTex);
	if (collider != nullptr)
		collider->toDelete = true;
	currentAnim = nullptr;
}

void Bullet::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == ColliderType::COLLIDER || c2->type == ColliderType::COLLIDER)
	{
		collider->toDelete;
		toDelete = true;

	}
}
