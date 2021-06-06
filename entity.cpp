#include "entity.h"

/**
	File	: entity.cpp
	Author	: Devin Arena
	Date	: 5/6/2021
	Purpose	: Stores entity information such as position, health, symbol.
			  Contains getters and setters for stored values and updates
			  information.
*/

/**
	Default constructor, takes in position arguments
*/
Entity::Entity(EntityType type, int x, int y) : type(type), x(x), y(y) {
	// Placeholder texture for entities
	for (auto i = 0; i < 3; i++)
		image.push_back(std::vector<char>());
	image[0] = { ' ', '@', ' ' };
	image[1] = { '@', '@', '@' };
	image[2] = { ' ', '@', ' ' };
	width = image[0].size();
	height = image.size();
}

/**
	Returns whether or not this entity is colliding with another.
	Probably one of the most complicated functions due to
	sprites being 2 dimensional arrays of characters.

	@return bool if colliding true else false
*/
bool Entity::collides(Entity* other) const {
	// loop over every character of the sprite
	for (auto j = 0; j < height; j++) {
 		for (auto i = 0; i < width; i++) {
			// ignore empty characters
			if (image[j][i] == ' ')
				continue;
			// get the actual position of the character on the screen
			int dx = getX() - (width / 2) + i;
			int dy = getY() - (height / 2) + j;
			// solve for the other entities j and i values
			int ni = dx - other->getX() + (other->width / 2);
			int nj = dy - other->getY() + (other->height / 2);
			// compares the i and j values to ensrue they are within image bounds
			// before returning true if the other character isn't empty and false otherwise
			if (nj < other->image.size() && ni < other->image[nj].size() && other->image[nj][ni] != ' ')
				return true;
		}
	}
	// default return case
	return false;
}

/**
	Contains basic update logic all entities follow such as
	position updating from velocity.
*/
void Entity::update(std::vector<Entity*>& entities) {
	x += vx * 0.1;
	y += vy * 0.1;
}

/**
	Custom render function that draws the 2d array of
	characters to the screen. The 2d array allows for much
	more complicated graphics to be displayed.

	@param screen wchar_t* the screen to draw to as a character array
*/
void Entity::render(wchar_t* screen) {
	// cool flashing effect if an entity is hurt
	if (hurt) {
		// check elapsed time since hurt
		uint64_t elapsed = currentTime() - hurtTimer;
		// no longer hurt after 1 second
		if (elapsed >= HURT_TIME)
			hurt = false;
		// actual flash logic, as timer goes up, see if its a factor of 3 and if so flash
		if (elapsed / 100 % 2 == 0)
			return;
	}
	// loop over every character in sprite
	for (auto j = 0; j < height; j++) {
		for (auto i = 0; i < width; i++) {
			char c = image[j][i];
			// ignore empty characters
			if (c == ' ')
				continue;
			// get the sprites actual position on screen
			int dx = getX() - (width / 2) + i;
			int dy = getY() - (height / 2) + j;
			// render the sprite onto the screen if within bounds
			if (dx >= 0 && dx < WIDTH && dy >= 0 && dy < HEIGHT) {
				screen[dy * WIDTH + dx] = c;
			}
		}
	}
}

/**
	Getter for x position.

	@return int x position
*/
int Entity::getX() const {
	return (int)round(this->x);
}

/**
	Getter for y position.

	@return int y position
*/
int Entity::getY() const {
	return (int)round(this->y);
}

/**
	Getter for x velocity.

	@return double x velocity
*/
double Entity::getVX() const {
	return this->vx;
}

/**
	Getter for y velocity.

	@return double y velocity
*/
double Entity::getVY() const {
	return this->vy;
}

/**
	Getter for entity health.

	@return int entity health
*/
int Entity::getHealth() const {
	return health;
}

/**
	Getter for entity max health.

	@return int entity max health
*/
int Entity::getMaxHealth() const {
	return maxHealth;
}

/**
	Getter for hurt state.

	@return bool true if hurt else false
*/
bool Entity::isHurt() const {
	return hurt;
}

/**
	Returns whether or not the entity is dead.
	Entities with health < 0 will never die.

	@return true if entity is dead else false
*/
bool Entity::isDead() const {
	return health == 0;
}

/**
	Gets the entity display image to render to the screen.

	@return 2d vector of characters
*/
std::vector<std::vector<char>> Entity::getImage() const {
	return image;
}

/**
	Gets the entity type (found in constants.h)

	@return the enum value of the entity type
*/
EntityType Entity::getType() const {
	return type;
}

/**
	Sets x position.

	@param x x position to set
	@return void
*/
void Entity::setX(double x) {
	this->x = x;
}

/**
	Sets y position.

	@param y y position to set
	@return void
*/
void Entity::setY(double y) {
	this->y = y;
}

/**
	Sets x velocity.

	@param x x velocity to set
	@return void
*/
void Entity::setVX(double vx) {
	this->vx = vx;
}

/**
	Sets y velocity.

	@param y y velocity to set
	@return void
*/
void Entity::setVY(double vy) {
	this->vy = vy;
}

/**
	Sets the entities health and clamps max health to it if necessary.

	@param health int the health to set to
*/
void Entity::setHealth(int health) {
	this->health = health;
	if (this->health > maxHealth) {
		maxHealth = health;
	}
}

/**
	Sets the entities max health.

	@param maxHealth int the maxHealth to set to
*/
void Entity::setMaxHealth(int maxHealth) {
	this->maxHealth = maxHealth;
}

/**
	Sets if the entity is hurt or not.

	@param hurt bool true if hurt else false
*/
void Entity::setHurt(bool hurt) {
	this->hurt = hurt;
}

/**
	Damages the entities for an amount and clamps health to 0 if necessary.

	@param damage int the damage to deal
*/
void Entity::damage(int damage) {
	health -= damage;
	if (health < 0)
		health = 0;
	hurt = true;
	hurtTimer = currentTime();
}