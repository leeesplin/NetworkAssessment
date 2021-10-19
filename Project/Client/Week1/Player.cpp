#include "Player.h"
#include <iostream>


Player::Player()
{
}


Player::~Player()
{
}

void Player::update(float dt, int enemyId)
{
	//gets the angle that the player is facing
	angleRadians = getRotation() * (3.141592653589793238463 / 180);

	//gets the direction that the player should move if they were to move forward in the direction they're facing
	direction.y = speed * cos(angleRadians);
	direction.x = speed * -sin(angleRadians);

	if (enemyId != -1)
	{
		// check what key is pressed and apply movement
		if (input->isKeyDown(sf::Keyboard::W))
		{
			//moves the player forward in the direction they're facing
			move(direction.x * dt, direction.y * dt);
		}

		if (input->isKeyDown(sf::Keyboard::A))
		{
			//rotates the player left
			rotate(-40 * dt);
		}

		if (input->isKeyDown(sf::Keyboard::S))
		{
			//moves the player backward in the direction they're facing
			move(-direction.x * dt, -direction.y * dt);
		}

		if (input->isKeyDown(sf::Keyboard::D))
		{
			//rotates the player right
			rotate(40 * dt);
		}
	}
}

void Player::checkDeath()
{
	if (health <= 0)
	{
		setAlive(false);
	}
	else
	{
		setAlive(true);
	}
}

void Player::collisionResponse(MySprite* sp, float dt)
{
	//cancels out the players movement direction if they are touching a wall in that direction
	if (input->isKeyDown(sf::Keyboard::W))
	{
		move(-direction.x * dt, -direction.y * dt);
	}

	if (input->isKeyDown(sf::Keyboard::S))
	{
		move(direction.x * dt, direction.y * dt);
	}
}

void Player::setHealth(int newHealth)
{
	health = newHealth;
}

int Player::getHealth()
{
	return health;
}