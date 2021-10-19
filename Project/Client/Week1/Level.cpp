#include "Level.h"

// The IP address for the server
#define SERVERIP "192.168.1.56"

Level::Level(sf::RenderWindow* hwnd, Input* input, GameState* gameState, EndScreen* endscreen)
{
	window = hwnd;
	inputObj = input;
	gameStateObj = gameState;
	endScreenObj = endscreen;

	setupGameObjects(window, inputObj, gameStateObj);


	//sets socket to non-blocking mode so that packets can be lost and the socket can carry on trying to recieve packets
	socket.setBlocking(false);
}

Level::~Level()
{

}

void Level::setupGameObjects(sf::RenderWindow* hwnd, Input* input, GameState* gameState)
{
	//load red and blue tank textures from file
	blueTankTex.loadFromFile("gfx/tankBlue.png");
	blueTankBarrelTex.loadFromFile("gfx/tankBlueBarrel.png");
	redTankTex.loadFromFile("gfx/tankRed.png");
	redTankBarrelTex.loadFromFile("gfx/tankRedBarrel.png");
	backgroundTex.loadFromFile("gfx/dirt.png");

	//set blue tank properties
	player[0].setSize(sf::Vector2f(40, 40));
	player[0].setPosition(100, 100);
	player[0].setTexture(&blueTankTex);
	player[0].setAlive(true);
	player[0].setInput(inputObj);
	player[0].setWindowBoundsX(hwnd->getSize().x);
	player[0].setWindowBoundsY(hwnd->getSize().y);
	player[0].setOrigin(player[0].getSize().x / 2, player[0].getSize().y / 2);
	player[0].setCollisionBox(sf::FloatRect(-player[0].getSize().x / 2, -player[0].getSize().y / 2, player[0].getSize().x, player[0].getSize().y));
	player[0].setTag("player"); //set tag to differentiate objects during collisions

	//set blue gun properties
	gun[0].setSize(sf::Vector2f(30, 10));
	gun[0].setTexture(&blueTankBarrelTex);
	gun[0].setInput(inputObj);
	gun[0].setOrigin(0, gun[0].getSize().y / 2);

	//set red tank properties
	player[1].setSize(sf::Vector2f(40, 40));
	player[1].setPosition(500, 500);
	player[1].setRotation(180);
	player[1].setTexture(&redTankTex);
	player[1].setAlive(true);
	player[1].setInput(inputObj);
	player[1].setWindowBoundsX(hwnd->getSize().x);
	player[1].setWindowBoundsY(hwnd->getSize().y);
	player[1].setOrigin(player[0].getSize().x / 2, player[0].getSize().y / 2);
	player[1].setCollisionBox(sf::FloatRect(-player[0].getSize().x / 2, -player[0].getSize().y / 2, player[0].getSize().x, player[0].getSize().y));
	player[1].setTag("player"); //set tag to differentiate objects during collisions

	//set red gun properties
	gun[1].setSize(sf::Vector2f(30, 10));
	gun[1].setTexture(&redTankBarrelTex);
	gun[1].setInput(inputObj);
	gun[1].setOrigin(0, gun[0].getSize().y / 2);

	//used to check if a bullet has been fired from either tank
	bulletFired[0] = false;
	bulletFired[1] = false;

	//used to measure the delay until a tank is able to fire another bullet
	fireCounter[0] = 0;
	fireCounter[1] = 0;

	rect.setTag("wall"); //set tag to differentiate objects during collisions

	// main wall boundaries
	setupWall(rect, 30, hwnd->getSize().y, 0, 0); //left 
	setupWall(rect, 30, hwnd->getSize().y, hwnd->getSize().x - 30, 0); //right
	setupWall(rect, hwnd->getSize().x, 30, 0, 0); //top
	setupWall(rect, hwnd->getSize().x, 30, 0, hwnd->getSize().y - 30); //bottom

	// centre pieces
	setupWall(rect, 30, 300, 200, 150); //left
	setupWall(rect, 30, 300, 550, 150); //right 
	setupWall(rect, 100, 30, 340, 150); //top
	setupWall(rect, 100, 30, 340, 420); //bottom

	// obstacle walls
	setupWall(rect, 100, 30, 0, 200); //upper left
	setupWall(rect, 70, 30, 130, 350); //lower left
	setupWall(rect, 100, 30, 700, 350); //lower right
	setupWall(rect, 100, 30, 550, 200); //upper right
	setupWall(rect, 30, 70, 375, 0); //upper middle
	setupWall(rect, 30, 70, 375, 530); //lower middle

	//set background properties
	background.setPosition(0, 0);
	background.setSize(sf::Vector2f(hwnd->getSize().x, hwnd->getSize().y));
	background.setTexture(&backgroundTex);

	//load font from file
	if (!font.loadFromFile("font/arial.ttf"))
	{
		std::cout << "Error loading font\n";
	}

	//set text properties
	for (int i = 0; i < 2; i++)
	{
		healthText[i].setFont(font);
		healthText[i].setCharacterSize(20);
		healthText[i].setFillColor(sf::Color::Green);
		healthText[i].setPosition(20, 20);
	}

	//set cooldown gui text properties for each tank
	for (int i = 0; i < 2; i++)
	{
		chargeText[i].setFont(font);
		chargeText[i].setCharacterSize(20);
		chargeText[i].setFillColor(sf::Color::White);
	}
	chargeText[0].setPosition(20, 20);
	chargeText[1].setPosition(520, 20);
}

//sets up all the walls within the level
void Level::setupWall(MySprite sprite, float width, float height, float startPosx, float startPosY)
{
	//sets wall properties
	sprite.setSize(sf::Vector2f(width, height));
	sprite.setPosition(startPosx, startPosY);
	sprite.setFillColor(sf::Color::Black);
	sprite.setCollisionBox(sf::FloatRect(0, 0, width, height));
	
	//loads wall into wall vector
	walls.push_back(sprite);
}


void Level::update(float deltaTime)
{		
	//checks if the server's current time has been initalised
	if (initClock == true)
	{
		clientLocalClock.restart();
		initClock = false;
	}

	//allows for movement of tank and gun for the tank assigned to the player
	if (playerId >= 0)
	{
		player[playerId].update(deltaTime, enemyPlayerId);
		gun[playerId].update(deltaTime);

		//checks what player has won once a player's health reaches 0
		if(player[0].getHealth() == 0)
		{
			endScreenObj->setBackground(1);
			gameStateObj->setCurrentState(State::END);
		}
		else if (player[1].getHealth() == 0)
		{
			endScreenObj->setBackground(0);
			gameStateObj->setCurrentState(State::END);
		}
	}

	bulletMgr.update(deltaTime); //updates the position of all alive bullets

	int k = 1; //variable used for getting the opposite player in the for loop
	for (int i = 0; i < 2; i++)
	{
		player[i].checkDeath(); //checks to see if either player should be dead

		//checks the collisions between the player's bullet and the enemy's tank
		if (Collision::checkBoundingBox(bulletMgr.getBullets().at(i), &player[k]))
		{
			bulletMgr.getBullets().at(i)->collisionResponse();
		}

		if (player[i].isAlive() == true)
		{
			gun[i].setPosition(player[i].getPosition()); //sets the position of both guns to the be on either player's tank

			for (int j = 0; j < walls.size(); j++)
			{
				//checks for collisions of players with walls in the level
				if (Collision::checkBoundingBox(&player[i], &walls.at(j)))
				{
					player[i].collisionResponse(&walls.at(j), deltaTime);
				}

				//checks collision between the bullet and the walls
				if (Collision::checkBoundingBox(bulletMgr.getBullets().at(i), &walls.at(j)))
				{
					bulletMgr.getBullets().at(i)->collisionResponse();
				}

			}
			//text that shows the player's health follows them around 
			healthText[i].setString(std::to_string(player[i].getHealth()));
			healthText[i].setPosition(player[i].getPosition().x - (player[i].getSize().x / 2), player[i].getPosition().y);
		}
		k--;
	}
	k = 1;

	//executes if the enemy player has been given a valid ID number from the server and enough network positions have been recieved so that
	//the next position could be predicted
	if (enemyPlayerId != -1 && client_network_messages.size() >= 3)
	{
		//uses linear prediction and interpolation to guess where the enemy's position should be
		player[enemyPlayerId].setPosition(linearPredictionPosition());
		player[enemyPlayerId].setRotation(rotationPrediction());

		if (client_bullet_messages.size() >= 3)
		{
			//uses linear prediction to guess the position of the enemy's bullets
			bulletMgr.getBullets().at(enemyPlayerId)->setPosition(linearPredictionBulletPosition() * deltaTime);
		}
	}

	//executes if the player has not found the IP address of the server
	if (serverIpFound == false)
	{
		//send a message every so often to every application on the network to tell them whether they are the server or not
		if (currentTime >= previousTime + 10)
		{
			sendBroadcastMessage();
			previousTime = clientLocalClock.getElapsedTime().asMilliseconds();
		}	
		receiveServerIpMessage(); //attempts to recieve the server's message telling the client that the server is trying to communicate
	}

	//once the server has been located, then start recieving tank data from them
	if (serverIpFound == true)
	{
		receiveMessage(); //recieve tank data from the server

		int delay = 0; //delay value used for controlling how many messages are sent per second

		if (playerId == 0)
		{
			delay = 200;
		}
		else if (playerId == 1)
		{
			delay = 100;
		}

		//send a message to the server every so often 
		if (currentTime >= previousTime + 10)
		{
			sendMessage(); //send a message to the server

			//records a point in time so that we can find out if a certain amount of time has passed since that recorded time
			previousTime = clientLocalClock.getElapsedTime().asMilliseconds();
		}
	}

	if (clockSynced == true)
	{
		//start the timer and add it to the elapsed time recieved from the server's clock accounting for the time it took to arrive at the client
		currentServerTime = serverTime + serverMainClock.getElapsedTime().asMilliseconds() + latency;
	}

	currentTime = clientLocalClock.getElapsedTime().asMilliseconds(); //gets the current time on client side
}

//sends a message to everyone on the network telling them if are the server or not
void::Level::sendBroadcastMessage()
{
	ServerIp serverMessage; //message to be sent to everybody on the network

	serverMessage.isServer = false; //tells everybody that this client is not the server

	sf::IpAddress recipient = sf::IpAddress::Broadcast; //sends to every IP address on the network
	unsigned short port = 54000; //port that the server will be on

	sf::Packet packet;
	packet << serverMessage.isServer;

	//sends packet
	switch (socket.send(packet, recipient, port))
	{
	case sf::Socket::Partial:
		//if there is a partial send, the whole packet must be sent again to prevent data corruption
		while (socket.send(packet, recipient, port) != sf::Socket::Done)
		{
		}
		break;
	case sf::Socket::Done:
		std::cout << "send successful" << std::endl;
		break;

	case sf::Socket::NotReady:
	//	std::cout << "socket not ready to send" << std::endl;;
		break;

	case sf::Socket::Disconnected:
		std::cout << "socket disconnected recieve" << std::endl;
		break;

	case sf::Socket::Error:
		std::cout << "socket error" << std::endl;
		break;

	default:
		std::cout << "something went wrong" << std::endl;
		break;
	}
}

void Level::receiveServerIpMessage()
{
	ServerIp msg;//message to be sent to clients
	sf::Packet packet; //packet to hold the message

	sf::IpAddress sender; //IP address of sender
	unsigned short port; //port of the sender


	//socket checks if there is a packet to recieve
	switch (socket.receive(packet, sender, port))
	{
	case sf::Socket::Partial:
		//clear the packet since we have no use for partial receives
		packet.clear();
		break;

	case sf::Socket::NotReady:
		//std::cout << "socket not ready to recieve" << std::endl;
		break;

	case sf::Socket::Disconnected:
		std::cout << "socket disconnected" << std::endl;
		break;

	case sf::Socket::Error:
		std::cout << "socket error IP" << std::endl;
		break;

	case sf::Socket::Done:
		//packet is broken down and values are taken from it
		packet >> msg.isServer;

		if (msg.isServer == true)
		{
			//lets the client know that the server has been found
			serverIp = sender;
			serverPort = port;
			serverIpFound = true;
		}

		break;

	default:
		std::cout << "something wrong" << std::endl;
		break;
	}
}

void Level::sendMessage()
{
		ClientMessage msg;
		msg.time = serverTime;
		msg.id = playerId;

		sf::Packet packet;

		//if the player has been assigned a tank to use then send all of the relevant values
		//if not then just send the id which tells the server that this client has no tank
		//assigned yet
		if (playerId >= 0)
		{
			//sends player properties
			msg.tankX = player[playerId].getPosition().x;
			msg.tankY = player[playerId].getPosition().y;	
			msg.health = player[playerId].getHealth();
			msg.playerRotation = player[playerId].getRotation();
			msg.gunRotation = gun[playerId].getRotation();
			msg.time = currentServerTime;
		
			//tells the server that a bullet has been fired by a player and is active in the scene
			if (bulletMgr.getBullets().at(playerId)->isAlive())
			{
				msg.bulletSpawn = true;
				msg.bulletRotation = bulletMgr.getBullets().at(playerId)->getRotation();
				msg.bulletX = bulletMgr.getBullets().at(playerId)->getPosition().x;
				msg.bulletY = bulletMgr.getBullets().at(playerId)->getPosition().y;
			}
			else
			{
				msg.bulletSpawn = false;
				msg.bulletRotation = -1;
				msg.bulletX = -1;
				msg.bulletY = -1;
			}

			//loads the packet with values
			packet << msg.playerRotation << msg.gunRotation << msg.bulletRotation << msg.bulletX << msg.bulletY 
				<< msg.bulletSpawn << msg.tankX << msg.tankY << msg.health << msg.time << msg.id;
		}
		else
		{
			packet << msg.id;
		}

		//sets ip and port numbers of the server
		sf::IpAddress recipient = serverIp;
		unsigned short port = serverPort;

		//sends the packet to the server
		switch (socket.send(packet, recipient, port))
		{
		case sf::Socket::Partial:
			//if there is a partial send, the whole packet must be sent again to prevent data corruption
			while (socket.send(packet, recipient, port))
			{
			}
			break;
		case sf::Socket::Done:
			std::cout << "send successful" << std::endl;
			break;

		case sf::Socket::NotReady:
	//		std::cout << "socket not ready to send" << std::endl;;
			break;

		case sf::Socket::Disconnected:
			std::cout << "socket disconnected SEND" << std::endl;
			break;

		case sf::Socket::Error:
			std::cout << "socket error" << std::endl;
			break;

		default:
			std::cout << "something went wrong" << std::endl;
			break;
		}
}

void Level::receiveMessage()
{
	ClientMessage msg;//message to be recieved
	sf::Packet packet; //packet to hold the message

	sf::IpAddress sender; //IP address of sender
	unsigned short port; //port of the sender


	//socket checks if there is a packet to recieve
	switch (socket.receive(packet, sender, port))
	{
	case sf::Socket::Partial:
		//clear the packet since we have no use for partial receives
		packet.clear();
		break;

	case sf::Socket::NotReady:
		//std::cout << "socket not ready to recieve" << std::endl;
		break;

	case sf::Socket::Disconnected:
		std::cout << "socket disconnected RECIEVE" << std::endl;
		window->close();
		break;

	case sf::Socket::Error:
		std::cout << "socket error" << std::endl;
		break;

	case sf::Socket::Done:
		packet >> msg.playerRotation >> msg.gunRotation >> msg.bulletRotation >> msg.bulletX >> msg.bulletY
			>> msg.bulletSpawn >> msg.tankX >> msg.tankY >> msg.health >> msg.time >> msg.id;

		//if the time of the latest message containing enemy data has not been recorded then record it
		if (latestRecievedTimeEnemy == 0 && msg.id == enemyPlayerId)
		{
			latestRecievedTimeEnemy = msg.time;
			gotLatestTime = true;
		}

		//if a duplicate packet has been detected then get rid of it
		if (msg.time <= latestRecievedTimeEnemy && msg.id == enemyPlayerId && gotLatestTime == false)
		{
			packet.clear();
		}
		else
		{
			//record the time of the latest message recieved
			if (msg.id == enemyPlayerId)
			{
				latestRecievedTimeEnemy = msg.time;
			}
			//syncs the server clock on server side with the server clock on client side if it hasn't been synced already
			if (clockSynced == false)
			{
				serverTime = msg.time;
				serverMainClock.restart();
				clockSynced = true;
			}

			//checks if the player has had an playerId given to them
			if (IDSet == false)
			{
				playerId = msg.id; //sets the player's ID to the ID given by the server
				IDSet = true;
			}

			//if the message id given to them is not the same as the player's id then the information sent to them is for the other player's tank
			if (playerId != msg.id)
			{
				//records the server time recieved from the previous message
				if (recordPreviousServerTime == true)
				{
					recordPreviousServerTime = false;

					if (previousReceivedServerTime == -1)
					{
						previousReceivedServerTime = msg.time; //records the current time recieved as the previous time since there is no previous time at the beginning
					}
					else
					{
						previousReceivedServerTime = nextReceivedServerTime;
					}

					recordNextServerTime = true;
				}

				//records the server time recieved from the current message
				if (recordNextServerTime == true)
				{
					recordNextServerTime = false;
					nextReceivedServerTime = msg.time;
					recordPreviousServerTime = true;
				}

				//finds the latency 
				if (nextReceivedServerTime >= 0)
				{
					latency = nextReceivedServerTime - previousReceivedServerTime;
				}
				else
				{
					latency = 1; //sets latency to 1 so the prediction can function properly
				}


				enemyPlayerId = msg.id; //sets the enemy player's id

				//get rid of the oldest network message when there are 3 of them
				if (client_network_messages.size() == 3)
				{
					client_network_messages.pop_back();
				}

				//add a new network message 
				if (client_network_messages.size() < 3)
				{
					client_network_messages.insert(client_network_messages.begin(), msg);
				}

				//sets enemy tank values
				if (client_network_messages.size() < 3)
				{
					//sets the enemy player's position until we have enough network messages to predict the next position
					player[msg.id].setPosition(sf::Vector2f(msg.tankX, msg.tankY));
					player[msg.id].setRotation(msg.playerRotation);
				}

				gun[msg.id].setRotation(msg.gunRotation);
				player[msg.id].setHealth(msg.health);
				bulletMgr.getBullets().at(msg.id)->setAlive(msg.bulletSpawn);

				if (bulletMgr.getBullets().at(msg.id)->isAlive() && client_bullet_messages.size() < 3)
				{
					//sets the enemy player's bullet position until we have enough network messages to predict the next position
					bulletMgr.getBullets().at(msg.id)->setPosition(sf::Vector2f(msg.bulletX, msg.bulletY));
					bulletMgr.getBullets().at(msg.id)->setRotation(msg.bulletRotation);
				}

				if (msg.bulletSpawn == true)
				{
					//add client bullet data to the client bullet messages vector
					//after there are 3 entries we can stop getting position data for the bullet because we have enough information to predict its path
					if (client_bullet_messages.size() < 3)
					{
						client_bullet_messages.insert(client_bullet_messages.begin(), msg);
					}
				}
			}
			if (playerId == msg.id)
			{
				//sets the player's health to what is recorded on the server side
				if (msg.health >= 0)
				{
					player[msg.id].setHealth(msg.health);
				}
			}
		}
		gotLatestTime = false;
		break;

	default:
		std::cout << "something wrong" << std::endl;
		break;
	}
}


sf::Vector2f Level::linearPredictionPosition()
{
	const int nSize = client_network_messages.size();

	//set temporary variables that hold references to the network messages
	const ClientMessage& msgNet0 = client_network_messages[nSize - 3];
	const ClientMessage& msgNet1 = client_network_messages[nSize - 2];
	const ClientMessage& msgNet2 = client_network_messages[nSize - 1];

	float predictedNetworkTankX = 0;
	float predictedNetworkTankY = 0;

	float velocityNetworkTankX = 0;
	float velocityNetworkTankY = 0;

	//calculates the velocity of the tank over a certain amount of time
	velocityNetworkTankX = (msgNet0.tankX - msgNet1.tankX) / (msgNet0.time - msgNet1.time);
	velocityNetworkTankY = (msgNet0.tankY - msgNet1.tankY) / (msgNet0.time - msgNet1.time);

	//calculates the predicted position of the tank
	predictedNetworkTankX = msgNet0.tankX + (velocityNetworkTankX * (currentServerTime - msgNet0.time));
	predictedNetworkTankY = msgNet0.tankY + (velocityNetworkTankY * (currentServerTime - msgNet0.time));
	
	float latencyInSeconds = latency / 1000; //converts the latency(ms) to seconds

	//interpolation between the current position and the predicted position
	float lerpX = lerp(player[enemyPlayerId].getPosition().x, predictedNetworkTankX, latencyInSeconds);
	float lerpY = lerp(player[enemyPlayerId].getPosition().y, predictedNetworkTankY, latencyInSeconds);
	
	return sf::Vector2f(lerpX, lerpY);
}

sf::Vector2f Level::linearPredictionBulletPosition()
{
	if(bulletMgr.getBullets().at(enemyPlayerId)->isAlive())
	{
		const int mSize = client_bullet_messages.size();

		//set temporary variables that hold references to the bullet network messages
		const ClientMessage& msg0 = client_bullet_messages[mSize - 3];
		const ClientMessage& msg1 = client_bullet_messages[mSize - 2];
		const ClientMessage& msg2 = client_bullet_messages[mSize - 1];

		float predictedBulletX = 0;
		float predictedBulletY = 0;

		float velocityBulletX = 0;
		float velocityBulletY = 0;

		//calculates the velocity of the bullet over a certain amount of time
		velocityBulletX = (msg0.bulletX - msg1.bulletX) / (msg0.time - msg1.time);
		velocityBulletY = (msg0.bulletY - msg1.bulletY) / (msg0.time - msg1.time);

		//calculates the predicted position of the bullet
		predictedBulletX = bulletMgr.getBullets().at(enemyPlayerId)->getPosition().x + (velocityBulletX * (msg0.time - msg1.time));
		predictedBulletY = bulletMgr.getBullets().at(enemyPlayerId)->getPosition().y + (velocityBulletY * (msg0.time - msg1.time));

		float latencyInSeconds = latency / 1000; //converts the latency(ms) to seconds

		float lerpX = lerp(bulletMgr.getBullets().at(enemyPlayerId)->getPosition().x, predictedBulletX, latencyInSeconds);
		float lerpY = lerp(bulletMgr.getBullets().at(enemyPlayerId)->getPosition().y, predictedBulletY, latencyInSeconds);

		return sf::Vector2f(lerpX, lerpY);
	}
	else
	{
		//if the bullet isn't alive then don't do any prediction
		client_bullet_messages.clear();
		return sf::Vector2f(-1, -1);
	}
	
}

float Level::rotationPrediction()
{
	const int nSize = client_network_messages.size();

	//set temporary variables that hold references to the network messages
	const ClientMessage& msgNet0 = client_network_messages[nSize - 3];
	const ClientMessage& msgNet1 = client_network_messages[nSize - 2];
	const ClientMessage& msgNet2 = client_network_messages[nSize - 1];

	float predictedRotation = 0;

	float networkRotationRate = 0;

	float tempPlayerRotation0 = msgNet0.playerRotation;
	float tempPlayerRotation1 = msgNet1.playerRotation;

	float rotationDifference = tempPlayerRotation0 - tempPlayerRotation1;

	if (rotationDifference > 350)
	{
		tempPlayerRotation0 -= 360;
	}

	if (rotationDifference < -350)
	{
		tempPlayerRotation0 += 360;
	}

	//calculates the velocity of the tank over a certain amount of time
	networkRotationRate = (tempPlayerRotation0 - tempPlayerRotation1) / (msgNet0.time - msgNet1.time);

	//calculates the predicted position of the tank
	predictedRotation = msgNet0.playerRotation + (networkRotationRate * (currentServerTime - msgNet0.time));

	float latencyInSeconds = latency / 1000; //converts the latency(ms) to seconds

	return predictedRotation;
}

float Level::lerp(float startPos, float endPos, float time)
{
	return startPos * (1.0f - time) + time * endPos;
}

void Level::handleInput(float deltaTime)
{
	if (enemyPlayerId != -1)
	{
		//fires bullet depending on player's input
		if (inputObj->isKeyDown(sf::Keyboard::Space))
		{
			if (playerId >= 0)
			{
				if (player[playerId].isAlive() == true)
				{
					if (bulletFired[playerId] == false)
					{
						//gets the angle (in radians then converted to degrees) between the player and the mouse to see in what direction to fire the bullet
						float angleRadians = atan2(inputObj->getMouseY() - player[playerId].getPosition().y, inputObj->getMouseX() - player[playerId].getPosition().x);
						float angleDegrees = angleRadians * (180 / PI);

						float speed = 200.0f;
						sf::Vector2f direction;

						//fires the bullet in that direction
						direction.y = speed * cos(angleRadians - (PI / 2));
						direction.x = speed * -sin(angleRadians - (PI / 2));

						//spawns the bullet
						bulletMgr.spawn(gun[playerId].getPosition(), inputObj, direction, angleDegrees, playerId);
						bulletFired[playerId] = true;
					}
				}
			}
		}

		for (int i = 0; i < 2; i++)
		{
			//bullet firing cooldown
			if (bulletFired[i] == true)
			{
				fireCounter[i] += deltaTime;
			}

			//when the countdown reaches 5, make the bullet that was shot disappear and allow the player to fire another bullet
			if (fireCounter[i] >= 5)
			{
				bulletMgr.getBullets().at(i)->setAlive(false);
				bulletFired[i] = false;
				fireCounter[i] = 0;
			}
		}

	}
	//set the charge text to show the fire counter on the screen
	chargeText[playerId].setString(std::to_string(fireCounter[playerId]));
}

void Level::render()
{
	beginDraw();

	window->draw(background); 

	for (int i = 0; i < 2; i++)
	{
		if (player[i].isAlive() == true)
		{
			//draw the player and the players health
			window->draw(player[i]);
			window->draw(gun[i]);
			window->draw(healthText[i]);
		}
	}
	bulletMgr.render(window); //renders all active bullets in the level	

	//draws all walls in the level
	for (int i = 0; i < walls.size(); i++)
	{
		window->draw(walls.at(i));
	}		

	//draws the player's charge counter
	window->draw(chargeText[playerId]);
	
	endDraw();
}

void Level::beginDraw()
{
	window->clear(sf::Color::Black);
}

void Level::endDraw()
{
	window->display();
}

