
#include "boss_hand.h"
#include "boss.h"

/**
	File	: boss_hand.cpp
	Author	: Devin Arena
	Date	: 5/22/2021
	Purpose	: The final boss, multiple attacks, shields, everything.
			  Kill him to win... if you can.
*/

/**
	Default constructor for Helios. Takes in position and velocity arguments.

	@param x int x position
	@param y int y position
	@param vx double x velocity
	@param vy double y velocity
*/
HeliosHand::HeliosHand() : Entity(EntityType::BOSS, 0, 0) {
	// Here I generate a sprite for each object using ascii art
	image.clear();
	for (auto i = 0; i < 6; i++)
		image.push_back(std::vector<char>());
	image[0] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', '@', '@', };
	image[1] = { ' ', '@', '@', '@', ' ', ' ', ' ', '@', '@', };
	image[2] = { '@', '@', '@', '@', '@', ' ', '@', '@', '@', };
	image[3] = { ' ', ' ', '+', '@', '@', '@', '@', '@', '@', };
	image[4] = { '@', '@', '@', '@', '@', '@', '@', '@', '@', };
	image[5] = { ' ', '@', '@', '@', ' ', ' ', ' ', ' ', ' ', };
	width = image[0].size();
	height = image.size();
	health = maxHealth = HELIOS::HAND_HEALTH;
	x = WIDTH - width / 2 - 1;
}

/**
	Update method, first runs parent method then
	checks for collisions with friendly bullets.

	@param &entities reference to the entity list from shooter.cpp
*/
void HeliosHand::update(std::vector<Entity*>& entities) {
	Entity::update(entities);

	// shooting logic, if SHOOT_TIME (millis) has passed, shoot and reset timer
	if (currentTime() - shootTimer >= HELIOS::SHOOT_TIME_E) {
		shootTimer = currentTime();
		shoot(entities);
	}

	if (hurt || boss->getShield() != nullptr) // ignore damage if hurt
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
					boss->handDied(this);
			}
		}
	}
}

/**
	Flips the hand image vertically, necessary since one is a top hand and one is a bottom.
*/
void HeliosHand::flipImage() {
	for (auto i = 0; i < height / 2; i++) {
		std::vector<char> temp = image[i];
		image[i] = image[image.size() - 1 - i];
		image[image.size() - 1 - i] = temp;
	}
}

/**
	Shooting logic for boss hand, fires an exploding enemy.

	@param entities& the list of entities to add to
*/
void HeliosHand::shoot(std::vector<Entity*>& entities) {
	int x = getX() - width / 2 + 3;
	int y = getY() - height / 2;
	Entity* exploder = new Exploder(x, y, -EXPLODER::SPEED, 0);
	exploder->setHealth(2);
	entities.push_back(exploder);
}

/**
	Sets up the pointer to the boss enemy.

	@param boss* the parent boss
*/
void HeliosHand::setBoss(Helios* boss) {
	this->boss = boss;
}