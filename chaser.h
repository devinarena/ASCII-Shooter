#ifndef SHOOTER_CHASER
#define SHOOTER_CHASER

#include "entity.h"

class Chaser : public Entity {
private:
	Entity* player{ nullptr };
public:
	// see chaser.cpp for function definitions
	// default constructor takes position/velocity arguments
	Chaser(int x, int y, double vx, double vy);
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
};

#endif