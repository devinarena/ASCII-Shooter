#include "trishooter.h"

/**
	File	: trishooter.cpp
	Author	: Devin Arena
	Date	: 5/20/2021
	Purpose	: A more specialized ship enemy that fires 3 bullets at a time.
*/

/**
	Default constructor for a TriShooter. Takes in position and velocity arguments.

	@param x int x position
	@param y int y position
	@param vx double x velocity
	@param vy double y velocity
*/
TriShooter::TriShooter(int x, int y, double vx, double vy) : BaseShip(x, y, vx, vy) {
	// Here I generate a sprite for each object using ascii art
	image.clear();
	for (auto i = 0; i < 3; i++)
		image.push_back(std::vector<char>());
	image[0] = { ' ', '-', '/' };
	image[1] = { '<', '@', '<' };
	image[2] = { ' ', '-', '\\' };
	width = image[0].size();
	height = image.size();
	health = maxHealth = TRISHOOTER::HEALTH;
}

/**
	Update method, calls parent update (movement) then checks
	collisions and shoots basic bullets.

	@param &entities reference to the entity list from shooter.cpp
*/
void TriShooter::update(std::vector<Entity*>& entities) {
	Entity::update(entities);

	if (x < -width / 2) // bounds checking
		health = maxHealth = 0;

	if (hurt) // ignore damage if hurt
		return;

	// loop over all entities
	for (auto i = 0; i < entities.size(); i++) {
		Entity* e = entities[i]; // reference pointer
		// only damaged by certain entities
		if (e->getType() == EntityType::PLAYER_BULLET) {
			// check if colliding
			if (collides(e)) {
				// kill other entity
				e->setHealth(0);
				// take damage
				damage(1);
			}
		}
	}

	// shooting logic, if SHOOT_TIME (millis) has passed, shoot and reset timer
	if (currentTime() - shootTimer >= TRISHOOTER::SHOOT_TIME) {
		shootTimer = currentTime();
		shoot(entities);
	}
}

/**
	Shooting logic for this ship type.
*/
void TriShooter::shoot(std::vector<Entity*>& entities) {
	for (int i = -1; i <= 1; i++) {
		Bullet* b = new Bullet(EntityType::ENEMY_BULLET, x - 3, y + i, -TRISHOOTER::SHOOT_SPEED, (i / 3.0), 1);
		entities.push_back(b);
	}
}