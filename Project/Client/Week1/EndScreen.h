#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <sstream>

#include "Input.h"
#include "MySprite.h"
#include "GameState.h"

class EndScreen
{
public:
	EndScreen(sf::RenderWindow* hwnd, Input* input, GameState* gameState);
	~EndScreen();

	void handleInput(float deltaTime);
	void update(float deltaTime);
	void render();
	void setBackground(int newBackground);

private:
	sf::RenderWindow* window;
	Input* inputObj;
	GameState* gameStateObj;

	sf::Texture backgroundTexBlue;
	sf::Texture backgroundTexRed;
	sf::RectangleShape backgroundImage;

	int winnerBackground = -1; //value used to determine which background to use

	void beginDraw();
	void endDraw();

	sf::Font font;
	sf::Text text;
};