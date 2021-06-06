#include "spawner.h"

/*
	File	: spawner.cpp
	Author	: Devin Arena
	Date	: 5/16/2021
	Purpose	: Spawns enemies based on an ingame timer.
*/

/**
	Default constructor, no arguments.
*/
Spawner::Spawner() {
}

/**
	Update method calculates the elapsed time (time since last spawn) and
	returns if not enough time has passed. If it has, the function increments
	spawnID and spawns an entity based on spawnID.

	@param entities &std:vector<Entity*> a reference to a list of entities to add to
*/
void Spawner::update(std::vector<Entity*>& entities) {
	// calculate elapsed time and do nothing if not enough time has passed
	uint64_t elapsed = currentTime() - spawnTimer;
	if (elapsed < SPAWN_TIME)
		return;
	// set the spawnTimer to now 
	spawnTimer = currentTime();
	// spawn an entitity based on the current id
	if (spawnID == 0) {
		entities.push_back(new Asteroid(WIDTH + 5, 3 + (rand() % validY), -ASTEROID::SPEED, 0));
	}
	else if (spawnID == 1) {
		entities.push_back(new BaseShip(WIDTH + 5, 3 + (rand() % validY), -BASICSHIP::SPEED, 0));
	}
	else if (spawnID == 2) {
		entities.push_back(new Rocket(WIDTH + 5, 3 + (rand() % validY), -ROCKET::SPEED, 0));
	}
	else if (spawnID == 3) {
		entities.push_back(new TriShooter(WIDTH + 5, 3 + (rand() % validY), -TRISHOOTER::SPEED, 0));
	}
	else if (spawnID == 4) {
		entities.push_back(new Sniper(WIDTH + 5, 3 + (rand() % validY), -SNIPER::SPEED, 0));
	}
	else if (spawnID == 5) {
		entities.push_back(new Chaser(WIDTH + 5, 3 + (rand() % validY), -CHASER::SPEED, 0));
	}
	else if (spawnID == 6) {
		entities.push_back(new Exploder(WIDTH + 5, 3 + (rand() % validY), -EXPLODER::SPEED, 0));
	}
	// increment spawn id
	spawnID++;
	// cycle spawned entities
	if (spawnID > 6)
		spawnID = 0;
}

/**
	Gets the current spawn time for enemies.

	@return int SPAWN_TIME time for each entity spawn
*/
int Spawner::getSpawnTime() const {
	return this->SPAWN_TIME;
}

/**
	Sets the spawn time for the spawner (used to enter hardmode).

	@param spawnTime int the amount of time between spawns
*/
void Spawner::setSpawnTime(int spawnTime) {
	this->SPAWN_TIME = spawnTime;
}