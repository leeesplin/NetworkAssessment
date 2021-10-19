#include "MySprite.h"

MySprite::MySprite()
{}

MySprite::~MySprite()
{}

// override this function to provide functionality
void MySprite::update(float dt)
{
}

// Sets the velocity of the sprite
void MySprite::setVelocity(sf::Vector2f vel)
{
	velocity = vel;
}
void MySprite::setVelocity(float vx, float vy)
{
	velocity.x = vx;
	velocity.y = vy;
}

// get sprite velocity
sf::Vector2f MySprite::getVelocity()
{
	return velocity;
}

// Returns collision box + position, giving the collision box in the correct position
sf::FloatRect MySprite::getCollisionBox() { 
	
	return sf::FloatRect(collisionBox.left + getPosition().x, collisionBox.top + getPosition().y, collisionBox.width, collisionBox.height); 
}

// Reponse function, what the sprite does based on collision
// Colliding object is passed in for information
// e.g. compare sprite positions to determine new velocity direction.
// e.g. checking sprite type (world, enemy, bullet etc) so response is based on that.
void MySprite::collisionResponse(MySprite * sp)
{
}

void MySprite::setWindowBoundsX(float newWindowX)
{
	windowBoundsX = newWindowX;
}

void MySprite::setWindowBoundsY(float newWindowY)
{
	windowBoundsY = newWindowY;
}

void MySprite::setTag(std::string newTag)
{
	tag = newTag;
}

std::string MySprite::getTag()
{
	return tag;
}
