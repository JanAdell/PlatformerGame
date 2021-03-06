#ifndef _PICKUP_H
#define _PICKUP_H

#include "Entity.h"

class Pickup : public Entity
{
public:
	Pickup(const fPoint position);
	~Pickup();
	bool Start() override;
	void PreUpdate(float dt)override {};
	void Update(float dt) override;
	void Draw()override;
	void CleanUp();
	bool Save(pugi::xml_node& node) const;
	bool Load(pugi::xml_node& node);
	virtual void OnCollision(Collider* c1, Collider* c2)override;


private:

};

#endif
