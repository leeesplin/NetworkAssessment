#include "Level.h"

// The IP address for the server
#define SERVERIP "192.168.1.56"

Level::Level(sf::RenderWindow* hwnd, Input* input, GameState* gameState)
{
	window = hwnd;
	inputObj = input;
	gameStateObj = gameState;

	setupGameObjects(window, inputObj, gameStateObj);

	//bind the socket to a port
	if (socket.bind(54000) != sf::Socket::Done)
	{
		std::cout << "socket bind failed" << std::endl;
	}

	//sets socket to non-blocking mode so that packets can be lost and the socket can carry on trying to recieve packets
	socket.setBlocking(false); 
}

Level::~Level()
{

}

void Level::setupGameObjects(sf::RenderWindow* hwnd, Input* input, GameState* gameState)
{
	//load textures from file
	blueTankTex.loadFromFile("gfx/tankBlue.png");
	blueTankBarrelTex.loadFromFile("gfx/tankBlueBarrel.png");
	redTankTex.loadFromFile("gfx/tankRed.png");
	redTankBarrelTex.loadFromFile("gfx/tankRedBarrel.png");
	backgroundTex.loadFromFile("gfx/dirt.png");

	//set blue player properties
	player[0].setSize(sf::Vector2f(40, 40));
	player[0].setPosition(100, 100);
	player[0].setTexture(&blueTankTex);
	player[0].setAlive(false);
	player[0].setInput(inputObj);
	player[0].setWindowBoundsX(hwnd->getSize().x);
	player[0].setWindowBoundsY(hwnd->getSize().y);
	player[0].setOrigin(player[0].getSize().x / 2, player[0].getSize().y / 2);
	player[0].setCollisionBox(sf::FloatRect(-player[0].getSize().x / 2, -player[0].getSize().y / 2, player[0].getSize().x, player[0].getSize().y));
	player[0].setTag("player"); //set tag to differentiate objects during collisions
	player[0].setId(0);

	gun[0].setSize(sf::Vector2f(30, 10));
	gun[0].setTexture(&blueTankBarrelTex);
	gun[0].setInput(inputObj);
	gun[0].setOrigin(0, gun[0].getSize().y / 2);

	//set red player properties
	player[1].setSize(sf::Vector2f(40, 40));
	player[1].setPosition(500, 500);
	player[1].setTexture(&redTankTex);
	player[1].setAlive(false);
	player[1].setInput(inputObj);
	player[1].setWindowBoundsX(hwnd->getSize().x);
	player[1].setWindowBoundsY(hwnd->getSize().y);
	player[1].setOrigin(player[0].getSize().x / 2, player[0].getSize().y / 2);
	player[1].setCollisionBox(sf::FloatRect(-player[0].getSize().x / 2, -player[0].getSize().y / 2, player[0].getSize().x, player[0].getSize().y));
	player[1].setTag("player"); //set tag to differentiate objects during collisions
	player[1].setId(1);

	gun[1].setSize(sf::Vector2f(30, 10));
	gun[1].setTexture(&redTankBarrelTex);
	gun[1].setInput(inputObj);
	gun[1].setOrigin(0, gun[0].getSize().y / 2);

	bulletFired[0] = false;
	bulletFired[1] = false;

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
}

//sets up all the walls within the level
void Level::setupWall(MySprite sprite, float width, float height, float startPosx, float startPosY)
{
	//sets wall properties
	sprite.setSize(sf::Vector2f(width, height));
	sprite.setPosition(startPosx, startPosY);
	sprite.setFillColor(sf::Color::Black);
	sprite.setCollisionBox(sf::FloatRect(0, 0, width, height));

	walls.push_back(sprite);
}


void Level::update(float deltaTime)
{	
	int j = 1;
	for (int i = 0; i < 2; i++)
	{
		player[i].checkDeath(); //check if either player has died

		if (player[i].isAlive() == true)
		{
			//check bullet collisions
			if (bulletMgr.getBullets().at(i)->isAlive())
			{
				if (Collision::checkBoundingBox(&player[j], bulletMgr.getBullets().at(i)))
				{
					bulletMgr.getBullets().at(i)->collisionResponse(&player[j]);
				}
				bulletMgr.getBullets().at(i)->update(deltaTime);
			}

			gun[i].update(deltaTime, player[i].getPosition());	

			for (int k = 0; k < walls.size(); k++)
			{
				//checks for collisions with bullets with objects in the level
				bulletMgr.update(deltaTime, &walls.at(k));
		
			}
			//text that shows the player's health follows them around 
			healthText[i].setString(std::to_string(player[i].getHealth()));
			healthText[i].setPosition(player[i].getPosition().x - (player[i].getSize().x / 2), player[i].getPosition().y);
		}
		j--;
	}
	j = 1;

	//recieve details from clients then send them the server's IP until there are 2 players
	if (ClientIDs.size() < 2)
	{
		if (currentTime >= previousTime + 10)
		{
			recieveClientDetails();
			sendServerIp();
		}
	}

	//sends a message to the client every 10 ms
	if (currentTime >= previousTime + 10)
	{
			if (messageRecieved == true)
			{
				if (ClientIDs.size() != 2)
				{
					//if there's one valid client that has connected, then send their packet to themselves
					sendMessageToClient();
				}
				else
				{
					//if there's twp valid clients that have connected, then send their packet to themselves and eachother
					sendMessageToClients();
				}
		}

		previousTime = serverMainClock.getElapsedTime().asMilliseconds(); //used for sending messages at time intervals
	}

	recieveMessage(deltaTime); //attempts to recieve a message from the client

	currentTime = serverMainClock.getElapsedTime().asMilliseconds(); //records the current time on server side
}

void Level::recieveClientDetails()
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
	//	std::cout << "socket not ready to recieve" << std::endl;
		break;

	case sf::Socket::Disconnected:
		std::cout << "socket disconnected" << std::endl;
		break;

	case sf::Socket::Error:
		std::cout << "socket error" << std::endl;
		break;

	case sf::Socket::Done:
		std::cout << "HEY" << std::endl;
		//packet is broken down and values are taken from it
		packet >> msg.isServer;
		if (msg.isServer == false)
		{
			//populate the first entry of the vector with the first client's IP that the server recieves a packet from
			if (connectedClientIPs.size() == 0)
			{
				connectedClientIPs.push_back(sender);
				sendToClient = true; //now allows the server to send messages to clients
			}

			//populate the first entry of the vector with the first client's port that the server recieves a packet from
			if (connectedClientPorts.size() == 0)
			{
				connectedClientPorts.push_back(port);
				sendToClient = true;
			}

			//records the sender's IP address and port number for later use
			if (connectedClientPorts.size() < 2)
			{
				for (int i = 0; i < connectedClientPorts.size(); i++)
				{
					if (connectedClientPorts.at(i) == port)
					{
						newClient = false;
					}
				}

				//if the port that the server recieves a message from is a new port or IP address then push it into the vector
				if (newClient == true)
				{
					connectedClientIPs.push_back(sender);
					connectedClientPorts.push_back(port);
					sendToClient = true;
				}

				newClient = true;
			}
		}

		break;

	default:
		std::cout << "something wrong" << std::endl;
		break;
	}
}

void Level::sendServerIp()
{
	ServerIp serverMessage;

	serverMessage.isServer = true; //lets other applications know that this is the server

	sf::IpAddress serverIp = sf::IpAddress::Broadcast; //broadcast the message to all IP addresses on the network

	sf::Packet packet;

	packet << serverMessage.isServer;

	//sends packet
	if (sendToClient == true)
	{
		switch (socket.send(packet, connectedClientIPs.at(connectedClientIPs.size() - 1), connectedClientPorts.at(connectedClientIPs.size() - 1)))
		{
		case sf::Socket::Partial:
			//if there is a partial send, the whole packet must be sent again to prevent data corruption
			while (socket.send(packet, serverIp, sf::Socket::AnyPort) != sf::Socket::Done)
			{
			}
			break;
		case sf::Socket::Done:
			std::cout << "send successful" << std::endl;
			sendToClient = false;
			break;

		case sf::Socket::NotReady:
			//std::cout << "socket not ready to send" << std::endl;;
			break;

		case sf::Socket::Disconnected:
			std::cout << "socket disconnected" << std::endl;
			break;

		case sf::Socket::Error:
			std::cout << "socket error" << std::endl;
			break;

		default:
			std::cout << "something went wrong" << std::endl;
			break;
		}
	}
}

void Level::recieveMessage(float dt)
{

	ClientMessage msg;//message to be sent to clients
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
		std::cout << "socket disconnected receive" << std::endl;
		window->close();
		break;

	case sf::Socket::Error:
		std::cout << "socket error" << std::endl;
		break;

	case sf::Socket::Done:
		//packet is broken down and values are taken from it
		packet >> msg.playerRotation >> msg.gunRotation >> msg.bulletRotation >> msg.bulletX >> msg.bulletY
			>> msg.bulletSpawn >> msg.tankX >> msg.tankY >> msg.health >> msg.time >> msg.id;

		if (msg.id >= 0)
		{
			//changes the tanks' values depending on what id is recieved from the message
			player[msg.id].setPosition(sf::Vector2f(msg.tankX, msg.tankY)); //sets player position on server side
			player[msg.id].setRotation(msg.playerRotation); //sets player rotation on server side
			gun[msg.id].setRotation(msg.gunRotation); //sets gun rotation on server side

			//if the bullet isn't alive on the client's side, then it's assumed that the player has not fired a bullet on server side
			if (bulletMgr.getBullets().at(msg.id)->isAlive() == false)
			{
				bulletFired[msg.id] = false;
			}
			
			//if the message says that the player has fired a bullet and there is no bullet fired on server side then spawn a bullet
			if (msg.bulletSpawn == true && bulletFired[msg.id] == false)
			{
				spawnBullet(msg.bulletRotation, msg.id, dt);
				bulletFired[msg.id] = true;
			}
		
			//if there are are no client IDs currently in the clientID vector, then populate it with the first valid ID recieved
			if (ClientIDs.size() == 0)
			{
				player[msg.id].setAlive(true);
				ClientIDs.push_back(msg.id);
			}
			else
			{
				//check to see if the message was recieved from a new client
				for (int i = 0; i < ClientIDs.size(); i++)
				{
					if (msg.id == ClientIDs.at(i))
					{
						registerID = false;
					}
				}

				//if the current ID recieved is not a duplicate ID then push it into the vector
				if (registerID == true)
				{
					player[msg.id].setAlive(true);
					ClientIDs.push_back(msg.id);
				}

				registerID = true;
			}
		}

		messageRecieved = true;
		break;

	default:
		std::cout << "something wrong" << std::endl;
		break;
	}
}

void Level::sendMessageToClient()
{
	//sends message to the clients
	ClientMessage msg;

	//sends client's message to itself so that it can get a valid ID number
	for (int i = 0; i < connectedClientPorts.size(); i++)
	{
		msg.tankX = player[i].getPosition().x;
		msg.tankY = player[i].getPosition().y;
		msg.playerRotation = player[i].getRotation();
		msg.gunRotation = gun[i].getRotation();
		msg.health = player[i].getHealth();
		msg.id = player[i].getId();

		//checks to see if any bullets have been fired, if there has then send that data across
		if (bulletMgr.getBullets().at(i)->isAlive())
		{
			msg.bulletRotation = bulletMgr.getBullets().at(i)->getRotation();
			msg.bulletX = bulletMgr.getBullets().at(i)->getPosition().x;
			msg.bulletY = bulletMgr.getBullets().at(i)->getPosition().y;
			msg.bulletSpawn = true;
		}
		else
		{
			msg.bulletRotation = -1;
			msg.bulletX = -1;
			msg.bulletY = -1;
			msg.bulletSpawn = false;
		}

		msg.time = serverMainClock.getElapsedTime().asMilliseconds(); //sends the server time to client so they can sync with the server clock

		sf::Packet packet;
		packet << msg.playerRotation << msg.gunRotation << msg.bulletRotation << msg.bulletX << msg.bulletY
			<< msg.bulletSpawn << msg.tankX << msg.tankY << msg.health << msg.time << msg.id;

		//sends packet
		switch (socket.send(packet, connectedClientIPs.at(i), connectedClientPorts.at(i)))
		{
		case sf::Socket::Partial:
			//if there is a partial send, the whole packet must be sent again to prevent data corruption
			while(socket.send(packet, connectedClientIPs.at(i), connectedClientPorts.at(i)) != sf::Socket::Done)
			{
			}
			break;
		case sf::Socket::Done:
			std::cout << "send successful" << std::endl;
			break;

		case sf::Socket::NotReady:
			std::cout << "socket not ready to send" << std::endl;;
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
}

void Level::sendMessageToClients()
{
	//sends message to the clients
	ClientMessage msg;

	//sends client's packets to themselves and eachother
	int j = connectedClientPorts.size() - 1;
	for (int i = 0; i < connectedClientPorts.size(); i++)
	{
		//gets properties of each tank
		msg.tankX = player[i].getPosition().x;
		msg.tankY = player[i].getPosition().y;
		msg.playerRotation = player[i].getRotation();
		msg.gunRotation = gun[i].getRotation();
		msg.health = player[i].getHealth();
		msg.id = player[i].getId();

		//checks to see if any bullets have been fired, if there has then send that data across
		if (bulletMgr.getBullets().at(player[i].getId())->isAlive())
		{
			msg.bulletRotation = bulletMgr.getBullets().at(player[i].getId())->getRotation();
			msg.bulletX = bulletMgr.getBullets().at(player[i].getId())->getPosition().x;
			msg.bulletY = bulletMgr.getBullets().at(player[i].getId())->getPosition().y;
			msg.bulletSpawn = true;
		}
		else
		{
			msg.bulletRotation = -1;
			msg.bulletX = -1;
			msg.bulletY = -1;
			msg.bulletSpawn = false;
		}

		msg.time = serverMainClock.getElapsedTime().asMilliseconds(); //sends the server time to client so they can sync with the server clock

		sf::Packet packet;
		packet << msg.playerRotation << msg.gunRotation << msg.bulletRotation << msg.bulletX << msg.bulletY
			<< msg.bulletSpawn << msg.tankX << msg.tankY << msg.health << msg.time << msg.id;


		//sends packet of player's data to enemy player
		switch (socket.send(packet, connectedClientIPs.at(j), connectedClientPorts.at(j)))
		{
		case sf::Socket::Partial:
			//if there is a partial send, the whole packet must be sent again to prevent data corruption
			while (socket.send(packet, connectedClientIPs.at(j), connectedClientPorts.at(j)) != sf::Socket::Done)
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
			std::cout << "socket disconnected" << std::endl;
			break;

		case sf::Socket::Error:
			std::cout << "socket error" << std::endl;
			break;

		default:
			std::cout << "something went wrong" << std::endl;
			break;
		}

		//sends packet from player's data to themselves
		switch (socket.send(packet, connectedClientIPs.at(i), connectedClientPorts.at(i)))
		{
		case sf::Socket::Partial:
			//if there is a partial send, the whole packet must be sent again to prevent data corruption
			while (socket.send(packet, connectedClientIPs.at(i), connectedClientPorts.at(i)) != sf::Socket::Done)
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
			std::cout << "socket disconnected" << std::endl;
			break;

		case sf::Socket::Error:
			std::cout << "socket error" << std::endl;
			break;

		default:
			std::cout << "something went wrong" << std::endl;
			break;
		}
		j--;
	}
}

void Level::spawnBullet(float rotation, int id, float dt)
{
	//rotates the bullet to face the way it's supposed to
	float angleRadians = ((rotation - 90)  * 3.14) / 180;
	float speed = 200.0f;
	sf::Vector2f direction;

	//calculates direction of the bullet
	direction.y = speed * cos(angleRadians - (PI / 2));
	direction.x = speed * -sin(angleRadians - (PI / 2));

	//sets bullet values
	bulletMgr.getBullets().at(id)->setDirection(direction);
	bulletMgr.getBullets().at(id)->setRotation(rotation);
	bulletMgr.getBullets().at(id)->setPosition(gun[id].getPosition() + sf::Vector2f(direction * 0.3f));
	bulletMgr.getBullets().at(id)->setAlive(true);
}

void Level::handleInput(float deltaTime)
{
}

void Level::render()
{
	beginDraw();

	window->draw(background); 
	
	bulletMgr.render(window); //renders all active bullets in the level	

	for (int i = 0; i < 2; i++)
	{
		if (player[i].isAlive() == true)
		{
			window->draw(player[i]);
			window->draw(gun[i]);
			window->draw(healthText[i]);
		}
	}

	//draws all walls in the level
	for (int i = 0; i < walls.size(); i++)
	{
		window->draw(walls.at(i));
	}		

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

