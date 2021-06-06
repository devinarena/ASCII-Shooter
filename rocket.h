#ifndef SHOOTER_ROCKET
#define SHOOTER_ROCKET

#include "entity.h"

class Rocket : public Entity {
protected:
	// nothing necessary here
public:
	// see rocket.cpp for function definitions
	// default constructor takes position/velocity arguments
	Rocket(int x, int y, double vx, double vy);
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
};

#endif