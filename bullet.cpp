
#include "bullet.h"

/**
	File	: bullet.cpp
	Author	: Devin Arena
	Date	: 5/11/2021
	Purpose	: Contains bullet properties and overloaded methods.
			  Bullets can either be from player or enemies.
			  Use type and vx to determine which type of bullet.
*/

/**
	Default bullet constructor, initializes type, position, velocity, damage.

	@param type EntityType to be
	@param x int x position
	@param y int y position
	@param vx int x velocity
	@param vy int y velocity
	@param int damage the damage to deal
*/
Bullet::Bullet(EntityType type, int x, int y, double vx, double vy, int damage) : Entity(type, x, y) {
	this->vx = vx;
	this->vy = vy;
	this->damage = damage;
	// Generate image for a bullet (1x1)
	image.clear();
	image.push_back(std::vector<char>());
	if (vx >= 0)
		image[0] = { '>' };
	else
		image[0] = { '<' };
	width = image[0].size();
	height = image.size();
	health = maxHealth = 1;
}

/**
	Update method simply checks the bullet is still in
	bounds before updating its position based on velocity.

	@param &entities reference to the entity list from shooter.cpp
*/
void Bullet::update(std::vector<Entity*>& entities) {
	if (x < -width / 2 || x > WIDTH - 1 + width / 2 || y < -height / 2 || y > HEIGHT - 1 + height / 2)
		health = 0;

	Entity::update(entities);

	// collision detection (bullets collide with other bullets)
	for (auto i = 0; i < entities.size(); i++) {
		Entity* e = entities[i]; // reference pointer
		// if bullet types don't match
		if (e->getType() != getType() && (e->getType() == EntityType::PLAYER_BULLET || e->getType() == EntityType::ENEMY_BULLET)) {
			// check if colliding
			if (collides(e)) {
				// take damage
				Entity::damage(e->getHealth());
				// kill other entity
				e->setHealth(0);
			}
		}
	}
}

/**
	Gets the amount of damage the bullet does.

	@return int the damage of the bullet
*/
int Bullet::getDamage() const {
	return damage;
}

/**
	Helper method to change the image since I need to a lot for bullets.

	@param icon char the character to use as the icon
*/
void Bullet::setImage(char icon) {
	image[0][0] = icon;
}