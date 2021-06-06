#include "shield.h"
#include "boss.h"

/**
	File	: shield.cpp
	Author	: Devin Arena
	Date	: 5/22/2021
	Purpose	: A shield for the boss to hide behind.
*/

/**
	Default constructor for a Shield. Takes in no arguments.
*/
Shield::Shield() : Entity(EntityType::BOSS, WIDTH - 23, 0) {
	// Here I generate a sprite for each object using ascii art
	image.clear();
	for (auto i = 0; i < HEIGHT; i++) {
		image.push_back(std::vector<char>());
		for (auto j = 0; j < 8; j++)
			image[i].push_back('#');
	}
	width = image[0].size();
	height = image.size();
	health = maxHealth = HELIOS::SHIELD_HEALTH;
	y = height / 2;
}

/**
	Update method, calls parent update then checks collisions.

	@param &entities reference to the entity list from shooter.cpp
*/
void Shield::update(std::vector<Entity*>& entities) {
	Entity::update(entities);

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
				if (health < 1)
					boss->shieldDied();
			}
		}
	}
}

/**
	Sets up the pointer to the boss enemy.

	@param boss* the parent boss
*/
void Shield::setBoss(Helios* boss) {
	this->boss = boss;
}