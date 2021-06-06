#include "player.h"

/**
	File	: player.cpp
	Author	: Devin Arena
	Date	: 5/6/2021
	Purpose	: Stores important information relevant to the
			  player and contains update processes.
*/

/**
	Default constructor for player, simply taking position.

	@param x starting x position
	@param y starting y position
*/
Player::Player(int x, int y) : Entity(EntityType::PLAYER, x, y) {
	health = maxHealth = PLAYER::HEALTH;
	// Here I generate a sprite for each image using ascii art
	image.clear();
	for (auto i = 0; i < 3; i++)
		image.push_back(std::vector<char>());
	image[0] = { ' ', '\\', '-', '-', '\\', ',', ' ' };
	image[1] = { ' ', '}', '+', '@', '+', '-', '=' };
	image[2] = { ' ', '/', '-', '-', '/', '\'', ' ' };
	width = image[0].size();
	height = image.size();
	HURT_TIME = 500;
}

/**
	Update function override, handles bulk of player logic.
*/
void Player::update(std::vector<Entity*>& entities) {
	// Entity update logic 
	Entity::update(entities);

	// Bounds checking to make sure player doesn't leave screen

	if (x < width / 2)
		x = width / 2;
	if (x > WIDTH - 1 - width / 2)
		x = WIDTH - 1 - width / 2;
	if (y < height / 2 + 1)
		y = height / 2 + 1;
	if (y > HEIGHT - 2 - height / 2)
		y = HEIGHT - 2 - height / 2;

	// For the slippery space effect, velocity doesn't zero instantly, it
	// decreases exponentially giving a sliding effect.

	vx *= PLAYER::SLIDE;
	vy *= PLAYER::SLIDE;

	// Shooting logic, fire bullet if attempting to shoot and the last
	// shot happened more than SHOOT_TIME milliseconds ago.

	if (shooting) {
		uint64_t now = currentTime(); // gets the current time in milliseconds
		image[1][6] = '>'; // just a nice sprite change to demonstrate when the player is shooting
		if (timer == 0 || now - timer >= PLAYER::SHOOT_TIME) { // check if the amount of elapsed time is >= SHOOT_TIME
			timer = currentTime(); // set the elapsed time to 0
			// Spawn a bullet at the nose of the ship
			Entity* bullet = new Bullet(EntityType::PLAYER_BULLET, x + width / 2 - 1, round(y), PLAYER::SHOOT_SPEED, 0, mDamage);
			entities.push_back(bullet);
		}
	}
	// reset the image if not shooting
	else {
		image[1][6] = '=';
	}

	// Cool animation for the flames coming out of the back of the ship

	for (auto i = 0; i < 3; i++)
		if (rand() % 8 == 0)
			image[i][0] = image[i][0] == ' ' ? '~' : ' ';

	// Here is collision detection for player

	if (hurt) // ignore damage if hurt
		return;

	// loop over all entities
	for (auto i = 0; i < entities.size(); i++) {
		Entity* e = entities[i]; // reference pointer
		// only damaged by certain entities
		if (e->getType() == EntityType::ENEMY || e->getType() == EntityType::ENEMY_BULLET) {
			// check if colliding
			if (collides(e)) {
				// kill other entity
				e->setHealth(0);
				e->setMaxHealth(0);
				// take damage
				damage(1);
			}
		}
	}
}

/**
	Getter for if the player is currently shooting.

	@return true if player is shooting else false
*/
bool Player::isShooting() const {
	return shooting;
}

/**
	Sets if the player is shooting or not.

	@param shooting whether or not the player is shooting
*/
void Player::setShooting(bool shooting) {
	this->shooting = shooting;
}

/**
	Getter for player damage.

	@return player's current damage
*/
int Player::getDamage() const {
	return mDamage;
}