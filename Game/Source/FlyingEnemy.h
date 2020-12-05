#ifndef _FLYINGENEMY_H
#define _FLYINGENEMY_H

#include "Enemy.h"

class FlyingEnemy : public Enemy
{
public:
	FlyingEnemy(const fPoint position);
	~FlyingEnemy();
	bool Start() override;
	void PreUpdate(float dt)override {};
	void Update(float dt) override;
	void Draw();
	void CleanUp();

private:
	bool go_right = true;
	float move_radius;
	fPoint initial_pos;
};

#endif
