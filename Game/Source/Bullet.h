#ifndef _BULLET_H
#define _BULLET_H

#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet(const fPoint position);
	~Bullet();
	bool Start() override;
	void PreUpdate(float dt)override {};
	void Update(float dt) override;
	void Draw()override;
	void CleanUp();
	virtual void OnCollision(Collider* c1, Collider* c2)override;


private:
	
	bool goRight;

};

#endif