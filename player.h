#ifndef SHOOTER_PLAYER
#define SHOOTER_PLAYER

#include "entity.h"
#include "bullet.h"
#include "shield.h"

class Player : public Entity {
private:
	int mDamage{ 0 };
	bool shooting{ false };
	uint64_t timer{ 0 };
public:
	// see player.cpp for function definitions
	// default constructor takes position arguments
	Player(int x, int y);
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
	// getters and setters
	bool isShooting() const;
	void setShooting(bool shooting);
	int getDamage() const;
};

#endif