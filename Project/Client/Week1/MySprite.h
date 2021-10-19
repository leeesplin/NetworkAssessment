#pragma once
#include "SFML\Graphics.hpp"
#include "Input.h"
#include <string.h>

class MySprite : public sf::RectangleShape
{
public:
	MySprite();
	~MySprite();

	virtual void update(float dt);

	// sprite speed and direction
	void setVelocity(sf::Vector2f vel);
	void setVelocity(float vx, float vy);
	sf::Vector2f getVelocity();

	// sprite state
	bool isAlive() { return alive; };
	void setAlive(bool b) { alive = b; };

	// for sprite collision
	bool isCollider() { return collider; };
	void setCollider(bool b) { collider = b; };
	sf::FloatRect getCollisionBox();
	void setCollisionBox(float x, float y, float width, float height) { collisionBox = sf::FloatRect(x, y, width, height); };
	void setCollisionBox(sf::FloatRect fr) { collisionBox = fr; };
	virtual void collisionResponse(MySprite* sp);

	// input component
	void setInput(Input* in) { input = in; };

	// window bounds
	void setWindowBoundsX(float newWindowX);
	void setWindowBoundsY(float newWindowY);

	void setTag(std::string newTag);
	std::string getTag();

protected:
	// Sprite properties
	sf::Vector2f velocity;
	bool alive;

	// Collision vars
	sf::FloatRect collisionBox;
	bool collider;

	// input component
	Input* input;

	float windowBoundsX = 0;
	float windowBoundsY = 0;

	std::string tag = ""; //tag used to identify the sprite
};