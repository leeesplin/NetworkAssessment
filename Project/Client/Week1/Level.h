#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

#include "Input.h"
#include "Player.h"
#include "Gun.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "MySprite.h"
#include "GameState.h"
#include "Collision.h"
#include "ClientMessage.h"
#include "EndScreen.h"

class Level 
{
public:
	Level(sf::RenderWindow* hwnd, Input* input, GameState* gameState, EndScreen* endscreen);
	~Level();

	void handleInput(float deltaTime); //handles all input from the player
	void setupWall(MySprite sprite, float width, float height, float startPosx, float startPosY); //creates a wall with the given parameters
	void update(float deltaTime);
	void sendMessage(); //sends a message to the server
	void receiveMessage(); //recieves a message from the server
	void receiveServerIpMessage(); //recieves a message from a potential server
	void sendBroadcastMessage(); //sends a message telling everyone on the network that they are not the server
	sf::Vector2f linearPredictionPosition(); //predicts the player's next position and interpolates 
	sf::Vector2f linearPredictionBulletPosition(); //predicts the bullets next position
	float rotationPrediction(); //predicts the rotation of the player
	float lerp(float startPos, float endPos, float time); //interpolates between 2 points
	void setupGameObjects(sf::RenderWindow* hwnd, Input* input, GameState* gameState);
	void render();

private:
	sf::RenderWindow* window;
	Input* inputObj;
	GameState* gameStateObj;
	EndScreen* endScreenObj;

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
	float fireCounter[2]; //keeps track of the cooldown for each player's gun

	sf::Font font;
	sf::Text healthText[2]; //health text that follows the players
	sf::Text chargeText[2]; //text that shows the weapon cooldown for each player

	sf::UdpSocket socket; //the client's socket

	sf::Clock clientLocalClock; //holds the clients local time
	float previousTime; //used for sending messages at intervals
	float currentTime;
	bool initClock = true; //check whether we want to start the clock or not

	//vectors that will hold the clients 3 latest messages received from the server 
	std::vector<ClientMessage> client_bullet_messages;
	std::vector<ClientMessage> client_network_messages;

	//records the latest time recieved by the server for the enemy's values
	//this is used to see if a duplicate packet has been received or not
	float latestRecievedTimeEnemy = 0;
	bool gotLatestTime = false;


	sf::Clock serverMainClock; //records the time of the server
	bool clockSynced = false; //checks whether the server's clock is synced on the client side
	float serverTime = 0; //value used for holding the server's current time when the client first recieves a message from it
	float currentServerTime; //use for holding the server's current time

	int playerId = -1; //holds the player's id
	int enemyPlayerId = -1; //holds the enemy's id
	bool IDSet = false; //used to check if the player has been assigned an id

	bool serverIpFound = false; //used to see if we have found the server's ip address yet
	sf::IpAddress serverIp; //holds the IP address of the server
	unsigned short serverPort; //holds the port number of the server

	//used for calculating latency
	bool recordPreviousServerTime = true;
	bool recordNextServerTime = false;
	float previousReceivedServerTime = -1;
	float nextReceivedServerTime = 0;
	float latency = 0;
};