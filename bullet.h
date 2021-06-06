#ifndef SHOOTER_PLAYER_BULLET
#define SHOOTER_PLAYER_BULLET

#include "entity.h"

class Bullet : public Entity {
private:
	int damage;
public:
	// see bullet.cpp for function definitions
	// default constructor takes entity type, position, velocity, and damage
	Bullet(EntityType type, int x, int y, double vx, double vy, int damage);
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
	// getters and setters
	void setImage(char image);
	int getDamage() const;
};

#endif
