
#include "asteroid.h"

/**
	File	: asteroid.cpp
	Author	: Devin Arena
	Date	: 5/12/2021
	Purpose	: The first enemy type, just a basic do nothing but move to the
			  left enemy that has a decent chunk of health.
*/

/**
	Default constructor for Asteroid. Takes in position and velocity arguments.

	@param x int x position
	@param y int y position
	@param vx double x velocity
	@param vy double y velocity
*/
Asteroid::Asteroid(int x, int y, double vx, double vy) : Entity(EntityType::ENEMY, x, y) {
	this->vx = vx;
	this->vy = vy;
	// Here I generate a sprite for each object using ascii art
	image.clear();
	for (auto i = 0; i < 5; i++)
		image.push_back(std::vector<char>());
	image[0] = { ' ', '@', '@', '@', '@', '@', ' ' };
	image[1] = { '@', '@', '@', '@', '@', '@', '@' };
	image[2] = { '@', '@', '@', '@', '@', '@', '@' };
	image[3] = { ' ', '@', '@', '@', '@', '@', '@' };
	image[4] = { ' ', ' ', '@', '@', ' ', '@', ' ' };
	width = image[0].size();
	height = image.size();
	health = maxHealth = ASTEROID::HEALTH;
}

/**
	Update method, first runs parent method then
	checks for collisions with friendly bullets.

	@param &entities reference to the entity list from shooter.cpp
*/
void Asteroid::update(std::vector<Entity*>& entities) {
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
}