#pragma once
#include "MySprite.h"

// Static class provide collision detection functions.
class Collision
{
public:
	//AABB
	static bool checkBoundingBox(MySprite* sp1, MySprite* sp2);
	// Bounding circle
	static bool checkBoundingSphere(MySprite* sp1, MySprite* sp2);

};