#ifndef SHOOTER_SHIELD
#define SHOOTER_SHIELD

class Helios;

#include "entity.h"

class Shield : public Entity {
protected:
	Helios* boss;
public:
	// see shield.cpp for function definitions
	// default constructor takes no arguments
	Shield();
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
	// getters and setters
	void setBoss(Helios* boss);
};

#endif