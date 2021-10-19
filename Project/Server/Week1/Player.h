#pragma once
#include "MySprite.h"
#include "Input.h"
#include "Vector.h"
#include <cmath>

class Player : public MySprite
{
public:
	Player();
	~Player();

	void update(float dt);

	void collisionResponse(MySprite *sp, float dt);
	void checkDeath();
	void setHealth(int newHealth);
	int getHealth();
	void setId(int newId);
	int getId();

private:
	float speed = 50.0f;
	sf::Vector2f direction;
	float angleRadians = 0; //the angle the player is facing in radians
	int health = 100;
	int id = -1;
};

