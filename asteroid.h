#ifndef SHOOTER_ASTEROID
#define SHOOTER_ASTEROID

#include "entity.h"

class Asteroid : public Entity {
private:
public:
	// see asteroid.cpp for function definitions
	// default constructor takes position/velocity arguments
	Asteroid(int x, int y, double vx, double vy);
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
};

#endif