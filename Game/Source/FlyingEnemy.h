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
	void Draw()override;
	void ChangeDir();
	void CleanUp();

	

private:
	bool goRight = true;
	float moveRadius;
	fPoint initialPos;
};

#endif
