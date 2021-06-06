#ifndef SHOOTER_TRISHOOTER
#define SHOOTER_TRISHOOTER

#include "basicship.h"

class TriShooter : public BaseShip {
protected:
	uint64_t shootTimer{ 0 };
public:
	// see trishooter.cpp for function definitions
	// default constructor takes position/velocity arguments
	TriShooter(int x, int y, double vx, double vy);
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
	virtual void shoot(std::vector<Entity*>& entities) override;
};

#endif