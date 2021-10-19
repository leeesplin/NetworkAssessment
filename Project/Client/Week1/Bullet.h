#pragma once
#include "MySprite.h"
#include "Input.h"
#include "Vector.h"
#include <cmath>

class Bullet : public MySprite
{
public:
	Bullet();
	~Bullet();

	void update(float dt);

	void collisionResponse();

	void setDirection(sf::Vector2f newDirection);

protected:
	sf::Vector2f direction;
	float speed = 50.0f;
	float angleRadians = 0; // the angle the bullet should be fired in
	float angleDegrees = 0;
};

