#include "Gun.h"


Gun::Gun()
{
}


Gun::~Gun()
{
}

void Gun::update(float dt, sf::Vector2f playerPos)
{
	//set the gun position to the origin of the player
	setPosition(playerPos.x, playerPos.y);
}

void Gun::collisionResponse(MySprite* sp)
{
	velocity.x = -velocity.x;
}