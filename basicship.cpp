#include "basicship.h"

/**
	File	: basicship.cpp
	Author	: Devin Arena
	Date	: 5/17/2021
	Purpose	: The most basic ship type of enemy, shoots bullets and
			  moves towards the player.
*/

/**
	Default constructor for a Basic Ship. Takes in position and velocity arguments.

	@param x int x position
	@param y int y position
	@param vx double x velocity
	@param vy double y velocity
*/
BaseShip::BaseShip(int x, int y, double vx, double vy) : Entity(EntityType::ENEMY, x, y) {
	this->vx = vx;
	this->vy = vy;
	// Here I generate a sprite for each object using ascii art
	image.clear();
	for (auto i = 0; i < 4; i++)
		image.push_back(std::vector<char>());
	image[0] = { ' ', ' ', ' ', '_', ' ' };
	image[1] = { ' ', '-', '/', '+', '\\' };
	image[2] = { '<', '@', '@', '@', '/' };
	image[3] = { ' ', '-', '-', '/', ' ' };
	width = image[0].size();
	height = image.size();
	health = maxHealth = BASICSHIP::HEALTH;
}

/**
	Update method, calls parent update (movement) then checks
	collisions and shoots basic bullets.

	@param &entities reference to the entity list from shooter.cpp
*/
void BaseShip::update(std::vector<Entity*>& entities) {
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
	if (currentTime() - shootTimer >= BASICSHIP::SHOOT_TIME) {
		shootTimer = currentTime();
		shoot(entities);
	}
}

/**
	Shooting logic for this ship type.
*/
void BaseShip::shoot(std::vector<Entity*>& entities) {
	Bullet* b = new Bullet(EntityType::ENEMY_BULLET, x - 3, y, -BASICSHIP::SHOOT_SPEED, 0, 1);
	entities.push_back(b);
}