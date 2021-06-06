#ifndef SHOOTER_SPAWNER
#define SHOOTER_SPAWNER

#include<vector>
#include<random>

#include "entity.h"
#include "asteroid.h"
#include "basicship.h"
#include "rocket.h"
#include "trishooter.h"
#include "sniper.h"
#include "chaser.h"
#include "exploder.h"

class Spawner {
private:
	int SPAWN_TIME{ 2000 };
	int validY { HEIGHT - 6 };
	int spawnID{ 0 };
	uint64_t spawnTimer{ 0 };
public:
	// see spawner.cpp for function definitions
	// default constructor takes no arguments
	Spawner();
	// member methods
	void update(std::vector<Entity*>& entities);
	// getters and setters
	int getSpawnTime() const;
	void setSpawnTime(int spawnTime);
};

#endif