
#include "Bullet.h"


Bullet::Bullet(const fPoint position) : Entity(position, "Bullet", ENTITY_TYPE::BULLET)
{
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
}

void Bullet::Draw()
{
}

void Bullet::CleanUp()
{
}
