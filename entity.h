#ifndef SHOOTER_ENTITY
#define SHOOTER_ENTITY

#include<vector>

#include "utils.h"

// more info in entity.cpp
class Entity {
protected:
	// variables all entities will use including position, display character, health
	int HURT_TIME{ 200 };
	double x{ 0 };
	double y{ 0 };
	double vx{ 0 };
	double vy{ 0 };
	int width{ 0 };
	int height{ 0 };
	std::vector<std::vector<char>> image = std::vector<std::vector<char>>();
	int health{ -1 };
	int maxHealth{ -1 };
	bool hurt{ false };
	uint64_t hurtTimer;
	EntityType type{ EntityType::ENEMY };
public:
	// see entity.cpp for function definitions
	// default constructor takes position arguments
	Entity(EntityType type, int x, int y);
	// member methods
	virtual void update(std::vector<Entity*>& entities);
	virtual void render(wchar_t* screen);
	bool collides(Entity* other) const;
	// getters and setters
	int getX() const;
	int getY() const;
	double getVX() const;
	double getVY() const;
	int getHealth() const;
	int getMaxHealth() const;
	bool isDead() const;
	bool isHurt() const;
	std::vector<std::vector<char>> getImage() const;
	EntityType getType() const;
	void setX(double x);
	void setY(double y);
	void setVX(double vx);
	void setVY(double vy);
	void setHealth(int health);
	void setMaxHealth(int health);
	void setHurt(bool hurt);
	void damage(int damage);
};

#endif