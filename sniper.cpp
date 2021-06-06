#include "sniper.h"

/**
	File	: sniper.cpp
	Author	: Devin Arena
	Date	: 5/20/2021
	Purpose	: A more specialized ship enemy that fires a targeted bullet.
*/

/**
	Default constructor for a Sniper. Takes in position and velocity arguments.

	@param x int x position
	@param y int y position
	@param vx double x velocity
	@param vy double y velocity
*/
Sniper::Sniper(int x, int y, double vx, double vy) : BaseShip(x, y, vx, vy) {
	// Here I generate a sprite for each object using ascii art
	image.clear();
	for (auto i = 0; i < 3; i++)
		image.push_back(std::vector<char>());
	image[0] = { ' ', ' ', ' ', ',', ' ', ' ', '/' };
	image[1] = { '<', '<', '<', '@', '+', '@', '{' };
	image[2] = { ' ', ' ', ' ', '\'', ' ', ' ', '\\' };
	width = image[0].size();
	height = image.size();
	health = maxHealth = SNIPER::HEALTH;
}

/**
	Update method, calls parent update (movement) then checks
	collisions and shoots basic bullets.

	@param &entities reference to the entity list from shooter.cpp
*/
void Sniper::update(std::vector<Entity*>& entities) {
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
	if (currentTime() - shootTimer >= SNIPER::SHOOT_TIME) {
		shootTimer = currentTime();
		shoot(entities);
	}
}

/**
	Shooting logic for this ship type.
*/
void Sniper::shoot(std::vector<Entity*>& entities) {
	Entity* player = entities[0]; // player is conventiently always the first entity
	// finds the angle to the player and shoots the bullet at that angle
	double angle = atan2(player->getY() - y, player->getX() - x);
	double vx = cos(angle) * SNIPER::SHOOT_SPEED;
	double vy = sin(angle) * SNIPER::SHOOT_SPEED;
	Bullet* b = new Bullet(EntityType::ENEMY_BULLET, x - 3, y, vx, vy, 1);
	b->setImage('+');
	entities.push_back(b);
}