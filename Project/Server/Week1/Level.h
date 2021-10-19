#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <sstream>

#include "Input.h"
#include "Player.h"
#include "Gun.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "MySprite.h"
#include "GameState.h"
#include "Collision.h"
#include "ClientMessage.h"

class Level 
{
public:
	Level(sf::RenderWindow* hwnd, Input* input, GameState* gameState);
	~Level();

	void handleInput(float deltaTime);
	void setupWall(MySprite sprite, float width, float height, float startPosx, float startPosY); //creates a wall with the given parameters
	void update(float deltaTime);
	void recieveMessage(float dt); //recieves messages from the client
	void recieveClientDetails(); //recieves initial details from the client
	void sendServerIp(); //sends the server details to clients
	void sendMessageToClient(); //sends message to the single connected client
	void sendMessageToClients(); //sends message to both clients
	void spawnBullet(float rotation, int id, float dt); //spawns a bullet
	void setupGameObjects(sf::RenderWindow* hwnd, Input* input, GameState* gameState);
	void render();

private:
	sf::RenderWindow* window;
	Input* inputObj;
	GameState* gameStateObj;

	void beginDraw();
	void endDraw();

	Player player[2]; //Player objects that represent the 2 players 
	Gun gun[2]; //Gun objects that represent the 2 players' guns
	BulletManager bulletMgr;

	MySprite rect;	//default rectangle sprite to be used for the walls
	MySprite background;

	//textures
	sf::Texture blueTankTex;
	sf::Texture blueTankBarrelTex;
	sf::Texture redTankTex;
	sf::Texture redTankBarrelTex;
	sf::Texture backgroundTex;

	std::vector<MySprite> walls; //vector used to store the walls

	float PI = 3.141592653589793238463;

	bool bulletFired[2]; //keeps track of what player has fired their gun
	float fireCounter[2]; //text that shows the weapon cooldown for each player

	sf::Font font;
	sf::Text healthText[2]; //health text that follows the players

	sf::UdpSocket socket;//the server's socket

	sf::Clock serverMainClock; //clock that keeps track of the server's local time
	float currentTime; //used for sending messages at intervals
	float previousTime;

	std::vector<sf::IpAddress> connectedClientIPs; //vector which holds connected client's IP addresses
	std::vector<unsigned short> connectedClientPorts; //vector which holds connected client's port numbers
	std::vector<int> ClientIDs; //vector which holds the IDs of connected clients

	bool messageRecieved = false; //records whether a message has been received or not
	bool newClient = true; //records whether a new client has connected or not
	bool registerID = true; //records whether we want to register the client's id or not

	bool sendToClient = false; //records whether we want to send the server's details to the clients yet
};