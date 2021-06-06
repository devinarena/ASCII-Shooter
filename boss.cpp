
#include "boss.h"

/**
	File	: boss.cpp
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
Helios::Helios() : Entity(EntityType::BOSS, 0, 0) {
	// Here I generate a sprite for each object using ascii art
	image.clear();
	for (auto i = 0; i < 9; i++)
		image.push_back(std::vector<char>());
	image[0] = { ' ', ' ', '@', '@', '@', '@', '@', '@', '@', '@' };
	image[1] = { ' ', '@', '@', '@', '@', '@', '@', '@', '@', '@' };
	image[2] = { '@', '@', '+', '@', '@', '@', '@', '@', '@', '@' };
	image[3] = { '@', '@', '@', '@', '@', '@', '@', '@', '@', '@' };
	image[4] = { '@', '@', '@', '@', '@', '@', '@', '@', '@', '@' };
	image[5] = { '@', '@', '@', '@', '@', '@', '@', '@', '@', '@' };
	image[6] = { '@', '@', '+', '@', '@', '@', '@', '@', '@', '@' };
	image[7] = { ' ', '@', '@', '@', '@', '@', '@', '@', '@', '@' };
	image[8] = { ' ', ' ', '@', '@', '@', '@', '@', '@', '@', '@' };
	width = image[0].size();
	height = image.size();
	health = maxHealth = HELIOS::HEALTH;
	x = WIDTH - width / 2;
	y = HEIGHT / 2 - height / 2 + 1;
}

/**
	Update method, first runs parent method then
	checks for collisions with friendly bullets.

	@param &entities reference to the entity list from shooter.cpp
*/
void Helios::update(std::vector<Entity*>& entities) {
	if (health < 1)
		return;

	if (player == nullptr) // player is always first entity
		player = entities[0];
	else {
		if (y < player->getY())
			vy = HELIOS::CHASE_SPEED;
		else if (y > player->getY())
			vy = -HELIOS::CHASE_SPEED;
		else
			vy = 0;
	}

	Entity::update(entities);

	if (hurt) // ignore damage if hurt
		return;

	// keep hands positioned correctly
	if (topHand != nullptr)
		topHand->setY(y - height / 2 - topHand->getImage().size() / 2);
	if (bottomHand != nullptr)
		bottomHand->setY(y + height / 2 + bottomHand->getImage().size() / 2 + 1);

	// if both hands are dead, create a new shield
	if (stage == 2) {
		newShield(entities);
		stage++;
	}

	if (stage >= 3) {
		// shooting normal bullets
		if (currentTime() - timerNormal >= HELIOS::SHOOT_TIME_N) {
			timerNormal = currentTime();
			for (int i = -numAttacks / 2; i <= numAttacks / 2; i++) {
				Bullet* b = new Bullet(EntityType::ENEMY_BULLET, x - 5, y, -HELIOS::SHOOT_SPEED_N, (i / 3.0), 1);
				entities.push_back(b);
			}
		}
		// shooting seeking bullets
		if (currentTime() - timerSeeking >= HELIOS::SHOOT_TIME_S) {
			timerSeeking = currentTime();
			// finds the angle to the player and shoots the bullet at that angle
			double angle = atan2(player->getY() - y, player->getX() - x);
			double vx = cos(angle) * HELIOS::SHOOT_SPEED_S;
			double vy = sin(angle) * HELIOS::SHOOT_SPEED_S;
			Bullet* b = new Bullet(EntityType::ENEMY_BULLET, x - 5, y, vx, vy, 1);
			b->setImage('+');
			entities.push_back(b);
		}
		// shooting exploding ships
		if (enraged) {
			if (currentTime() - timerExploding >= HELIOS::SHOOT_TIME_E) {
				timerExploding = currentTime();
				Entity* exploder = new Exploder(x - 5, y, -EXPLODER::SPEED, 0);
				entities.push_back(exploder);
			}
		}
	}

	// prevent damaging if hands aren't dead or a shield exists
	if (topHand != nullptr || bottomHand != nullptr || shield != nullptr)
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
				if (health < HELIOS::HEALTH / 2 && health + 1 >= HELIOS::HEALTH / 2)
					enrage();
				if (health < HELIOS::HEALTH * 0.33 && health + 1 > HELIOS::HEALTH * 0.33)
					newShield(entities);
			}
		}
	}
}

/**
	Called when a hand dies, just points to nothing, if both are dead, create a new shield.

	@param hand HeliosHand* the hand that died
*/
void Helios::handDied(HeliosHand* hand) {
	if (hand == topHand)
		topHand = nullptr;
	else if (hand == bottomHand)
		bottomHand = nullptr;
	stage++;
}

/**
	Called when shield dies, just points to nothing.
*/
void Helios::shieldDied() {
	shield = nullptr;
}

/**
	Creates a new shield for the boss.

	@param entities& entity list to add the shield to
*/
void Helios::newShield(std::vector<Entity*>& entities) {
	shield = new Shield();
	shield->setBoss(this);
	entities.push_back(shield);
}

/**
	Enrages the boss, ~doubles firing speed and increases bullet speed.
*/
void Helios::enrage() {
	enraged = true;
	image[2][4] = '/';
	image[6][4] = '\\';
	HELIOS::CHASE_SPEED = 4;
	HELIOS::SHOOT_TIME_N *= 0.65;
	HELIOS::SHOOT_TIME_S *= 0.65;
	HELIOS::SHOOT_TIME_E *= 0.8;
	HELIOS::SHOOT_SPEED_N = 14;
	HELIOS::SHOOT_SPEED_S = 17;
	stage++;
}

/**
	Getter for current boss stage.

	@return int the stage the boss is in
*/
int Helios::getStage() const {
	return stage;
}

/**
	Getter for Helios' top hand.

	@return a pointer to Helios' top hand
*/
HeliosHand* Helios::getTopHand() const {
	return topHand;
}

/**
	Getter for Helios' bottom hand.

	@return a pointer to Helios' bottom hand
*/
HeliosHand* Helios::getBottomHand() const {
	return bottomHand;
}

/**
	Getter for Helios' shield.

	@return a pointer to Helios' shield
*/
Shield* Helios::getShield() const {
	return shield;
}

/**
	Sets the top hand pointer.

	@param topHand HeliosHand* pointer to top hand
*/
void Helios::setTopHand(HeliosHand* topHand) {
	this->topHand = topHand;
}

/**
	Sets the top hand pointer.

	@param topHand HeliosHand* pointer to top hand
*/
void Helios::setBottomHand(HeliosHand* bottomHand) {
	this->bottomHand = bottomHand;
}

/**
	Sets the shield pointer.

	@param shield HeliosHand* pointer to top hand
*/
void Helios::setShield(Shield* shield) {
	this->shield = shield;
}