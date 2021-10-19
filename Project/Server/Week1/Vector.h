#pragma once
#include "SFML\System\Vector2.hpp"
#include <math.h>

class Vector
{
public:
	// Added as no function for normalising vectors
	static sf::Vector2f normalise(const sf::Vector2f &source);
	//Vector magnitude
	static float magnitude(sf::Vector2f vec);

private:
	Vector();
	~Vector();
};

