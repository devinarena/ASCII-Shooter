
#include "exploder.h"

/**
	File	: exploder.cpp
	Author	: Devin Arena
	Date	: 5/22/2021
	Purpose	: A bomb that explodes once it gets close enough to the player.
*/

/**
	Default constructor for Exploder. Takes in position and velocity arguments.

	@param x int x position
	@param y int y position
	@param vx double x velocity
	@param vy double y velocity
*/
Exploder::Exploder(int x, int y, double vx, double vy) : Entity(EntityType::ENEMY, x, y) {
	this->vx = vx;
	this->vy = vy;
	// Here I generate a sprite for each object using ascii art
	image.clear();
	for (auto i = 0; i < 3; i++)
		image.push_back(std::vector<char>());
	image[0] = { '_', '_', '_' };
	image[1] = { '|', '?', '|' };
	image[2] = { '|', '-', '|' };
	width = image[0].size();
	height = image.size();
	health = maxHealth = EXPLODER::HEALTH;
}

/**
	Update method, first runs parent method then
	checks for collisions with friendly bullets.
	Exploder explodes once its 15 tiles away from
	the player.

	@param &entities reference to the entity list from shooter.cpp
*/
void Exploder::update(std::vector<Entity*>& entities) {
	if (player == nullptr) // player is always first entity
		player = entities[0];

	// explode when 15 tiles away

	if (abs(player->getX() - x) <= 15)
		explode(entities);

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

/**
	Destroys this entity and creates a circle of 8 pieces
	of shrapnel that can hurt the player.

	@param &entities the entity list to add to
*/
void Exploder::explode(std::vector<Entity*>& entities) {
	for (auto i = 0; i < 8; i++) {
		double vx = cos(PI / 4.0 * i) * EXPLODER::PIECE_SPEED;
		double vy = sin(PI / 4.0 * i) * EXPLODER::PIECE_SPEED / 2;
		Bullet* shrapnel = new Bullet(EntityType::ENEMY_BULLET, x, y, vx, vy, 1);
		shrapnel->setImage('.');
		entities.push_back(shrapnel);
	}
	health = maxHealth = 0;
}