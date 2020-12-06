#include "Pickup.h"


Pickup::Pickup(const fPoint position) : Entity(position, "Pickup", EntityType::BULLET)
{
}

Pickup::~Pickup()
{
}

bool Pickup::Start()
{
	return true;
}

void Pickup::Update(float dt)
{
}

void Pickup::Draw()
{
}

void Pickup::CleanUp()
{
}