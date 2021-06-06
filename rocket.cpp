#include "rocket.h"

/**
	File	: rocket.cpp
	Author	: Devin Arena
	Date	: 5/18/2021
	Purpose	: A ramming enemy that moves quickly at the player.
*/

/**
	Default constructor for a Rocket. Takes in position and velocity arguments.

	@param x int x position
	@param y int y position
	@param vx double x velocity
	@param vy double y velocity
*/
Rocket::Rocket(int x, int y, double vx, double vy) : Entity(EntityType::ENEMY, x, y) {
	this->vx = vx;
	this->vy = vy;
	// Here I generate a sprite for each object using ascii art
	image.clear();
	for (auto i = 0; i < 3; i++)
		image.push_back(std::vector<char>());
	image[0] = { ' ', '<', '=', '=', '=', '=', '{' };
	image[1] = { '<', '=', '@', '+', '@', '=', '{' };
	image[2] = { ' ', '<', '=', '=', '=', '=', '{' };
	width = image[0].size();
	height = image.size();
	health = maxHealth = ROCKET::HEALTH;
}

/**
	Update method, calls parent update (movement) then checks collisions.

	@param &entities reference to the entity list from shooter.cpp
*/
void Rocket::update(std::vector<Entity*>& entities) {
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