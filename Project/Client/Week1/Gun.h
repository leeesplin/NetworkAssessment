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

	void update(float dt);

	void collisionResponse(MySprite *sp);

private:
	//angle the gun should be facing
	float angleRadians = 0;
	float angleDegrees = 0;
};

