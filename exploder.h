#ifndef SHOOTER_EXPLODER
#define SHOOTER_EXPLODER

#include "entity.h"
#include "bullet.h"
#include<math.h>

class Exploder : public Entity {
private:
	Entity* player{ nullptr };
public:
	// see chaser.cpp for function definitions
	// default constructor takes position/velocity arguments
	Exploder(int x, int y, double vx, double vy);
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
	void explode(std::vector<Entity*>& entities);
};

#endif