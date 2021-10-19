#include "BulletManager.h"

BulletManager::BulletManager()
{
	// some default values
	spawnPoint = sf::Vector2f(350, 250);

	blueBulletTex.loadFromFile("gfx/blueBullet.png");
	redBulletTex.loadFromFile("gfx/redBullet.png");

	for (int i = 0; i < 2; i++)
	{
		bullets.push_back(Bullet());
		bullets[i].setTag("bullet");
		bullets[i].setAlive(false);
		bullets[i].setPosition(-1, -1);
		bullets[i].setSize(sf::Vector2f(10, 10));
		bullets[i].setCollisionBox(0, 0, 10, 10);
	}
	bullets[0].setTexture(&blueBulletTex);
	bullets[1].setTexture(&redBulletTex);
}


BulletManager::~BulletManager()
{
}

void BulletManager::update(float dt, MySprite* sprite)
{
	//call update on all alive balls
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].isAlive())
		{
			collisionCheck(&bullets[i], sprite);
		}
	}
}

//spawn new bullet
//find a dead bullet, make alive, move to spawn point, move in given direction
void BulletManager::spawn(int location)//sf::Vector2f gunPos, Input* inputObj, sf::Vector2f direction, float rotation)
{
	if (!bullets[location].isAlive())
	{
		bullets[location].setAlive(true);
		//bullets[i].setPosition(gunPos + sf::Vector2f(direction * 3.5f)); //spawns the player in front of the tank barrel so it doesn't collide with the tank
		//bullets[i].setDirection(direction);
		//bullets[i].setRotation(rotation);
		//bullets[i].setInput(inputObj);
		return;
	}
}

void BulletManager::collisionCheck(Bullet* bullet, MySprite* sprite)
{
	if (Collision::checkBoundingBox(sprite, bullet))
	{
		if (sprite->getTag() == "player")
		{
			//removes 25 from player's health once hit
			Player* newSprite = static_cast<Player*>(sprite);
			newSprite->setHealth(newSprite->getHealth() - 25);
			sprite = newSprite;
		}
		bullet->setAlive(false);
		//bullet->setPosition(-1, -1);
	}
}


//render all alive bullets
void BulletManager::render(sf::RenderWindow* window)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].isAlive())
		{
			window->draw(bullets[i]);
		}
	}
}


std::vector<Bullet*> BulletManager::getBullets()
{
	std::vector<Bullet*> bulletList;
	for (int i = 0; i < bullets.size(); i++)
	{
		bulletList.push_back(&bullets.at(i));
	}

	return bulletList;
}
