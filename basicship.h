#ifndef SHOOTER_BASICSHIP
#define SHOOTER_BASICSHIP

#include "entity.h"
#include "bullet.h"

class BaseShip : public Entity {
protected:
	uint64_t shootTimer{ 0 };
public:
	// see baseship.cpp for function definitions
	// default constructor takes position/velocity arguments
	BaseShip(int x, int y, double vx, double vy);
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
	virtual void shoot(std::vector<Entity*>& entities);
};

#endif