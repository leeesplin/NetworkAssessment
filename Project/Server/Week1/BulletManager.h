#pragma once
#include "Bullet.h"
#include "MySprite.h"
#include "Collision.h"
#include "Player.h"
#include <math.h>
#include <vector>
#include <iostream>

class BulletManager
{
public:
	BulletManager();
	//BulletManager(Input* inputObj);
	~BulletManager();
	
	void spawn(int location);// sf::Vector2f gunPos, Input* inputObj, sf::Vector2f direction, float rotation);
	void update(float dt, MySprite* sprite);
	void collisionCheck(Bullet* bullet, MySprite* sprite);
	void render(sf::RenderWindow* window);
	std::vector<Bullet*> getBullets();

private:
	std::vector<Bullet> bullets;
	sf::Vector2f spawnPoint; //bullet spawn point
	sf::Texture blueBulletTex;
	sf::Texture redBulletTex;
};

