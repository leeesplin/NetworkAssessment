#include "Gun.h"


Gun::Gun()
{
}


Gun::~Gun()
{
}

void Gun::update(float dt)
{
	//gets angle between the fun and the mouse
	angleRadians = atan2(input->getMouseY() - getPosition().y, input->getMouseX() - getPosition().x);
	angleDegrees = angleRadians * (180 / 3.141592653589793238463);

	//rotate the gun to face the mouse pointer
	setRotation(angleDegrees);
}

void Gun::collisionResponse(MySprite* sp)
{
	velocity.x = -velocity.x;
}