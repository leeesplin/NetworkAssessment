#pragma once
#include "MySprite.h"
#include "Input.h"
#include "Vector.h"
#include <cmath>

class Gun : public MySprite
{
public:
	Gun();
	~Gun();

	void update(float dt, sf::Vector2f playerPos);

	void collisionResponse(MySprite *sp);

private:
	sf::Vector2f direction;
	float angleRadians = 0;
	float angleDegrees = 0;
};

