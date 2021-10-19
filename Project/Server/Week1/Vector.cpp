#include "Vector.h"

Vector::Vector()
{}

Vector::~Vector()
{}

sf::Vector2f Vector::normalise(const sf::Vector2f &source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

float Vector::magnitude(sf::Vector2f vec)
{
	return sqrt((vec.x*vec.x) + (vec.y*vec.y));
}




