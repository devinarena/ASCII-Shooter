#ifndef SHOOTER_HELIOSHAND
#define SHOOTER_HELIOSHAND

#include "entity.h"
#include "bullet.h"
#include "exploder.h"

class Helios;

class HeliosHand : public Entity {
private:
	uint64_t shootTimer{ 0 };
	Helios* boss{ nullptr };
public:
	// see boss.cpp for function definitions
	// default constructor takes position arguments
	HeliosHand();
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
	void shoot(std::vector<Entity*>& entities);
	void flipImage();
	// getters and setters
	void setBoss(Helios* boss);
};

#endif