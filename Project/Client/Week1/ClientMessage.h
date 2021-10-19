#pragma once
//a message to tell the server what the values of the player should be
struct ClientMessage
{
	float playerRotation; //rotation of the tank body
	float gunRotation; // rotation of the tank barrel;
	float bulletRotation; // rotation of the tank bullet
	float bulletX; //bullet position
	float bulletY;
	bool bulletSpawn; //whether the bullet should be alive or not
	float tankX; //tank position
	float tankY;
	int health; //tank's health
	float time; //time the message was sent
	int id; //id of the player
};

struct ServerIp
{
	bool isServer; //whether this application is the server or not
};