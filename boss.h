#ifndef SHOOTER_HELIOS
#define SHOOTER_HELIOS

#include "entity.h"
#include "bullet.h"
#include "boss_hand.h"
#include "shield.h"
#include "exploder.h"

class Helios : public Entity {
private:
	int stage{ 0 };
	int attack{ 0 };
	int numAttacks{ 5 };
	Entity* player{ nullptr };
	HeliosHand* topHand{ nullptr };
	HeliosHand* bottomHand{ nullptr };
	Shield* shield{ nullptr };
	uint64_t timerNormal{ 0 };
	uint64_t timerSeeking{ 0 };
	uint64_t timerExploding{ 0 };
	bool enraged{ false };
public:
	// see boss.cpp for function definitions
	// default constructor takes position arguments
	Helios();
	// member methods
	virtual void update(std::vector<Entity*>& entities) override;
	void handDied(HeliosHand* hand);
	void shieldDied();
	void newShield(std::vector<Entity*>& entities);
	void enrage();
	// getters and setters
	int getStage() const;
	HeliosHand* getTopHand() const;
	HeliosHand* getBottomHand() const;
	Shield* getShield() const;
	void setTopHand(HeliosHand* topHand);
	void setBottomHand(HeliosHand* bottomHand);
	void setShield(Shield* shield);
};

#endif