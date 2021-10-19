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

void Bullet::collisionResponse(Player* player)
{
	//take health off of player and destroy bullet
	player->setHealth(player->getHealth() - 25);
	setAlive(false);
}

void Bullet::setDirection(sf::Vector2f newDirection)
{
	direction = newDirection;
}