#include "Bullet.h"


Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::update(float dt)
{
	//move in given direction
	move(direction.x * dt, direction.y * dt);
}

void Bullet::collisionResponse()
{
	if (isAlive())
	{
		setAlive(false);
	}
}

void Bullet::setDirection(sf::Vector2f newDirection)
{
	direction = newDirection;
}