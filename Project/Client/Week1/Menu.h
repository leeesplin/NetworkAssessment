#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <sstream>

#include "Input.h"
#include "MySprite.h"
#include "GameState.h"

class Menu
{
public:
	Menu(sf::RenderWindow* hwnd, Input* input, GameState* gameState);
	~Menu();

	void handleInput(float deltaTime);
	void update(float deltaTime);
	void render();

private:
	sf::RenderWindow* window;
	Input* inputObj;
	GameState* gameStateObj;

	sf::Texture backgroundTex;
	sf::RectangleShape backgroundImage;

	void beginDraw();
	void endDraw();

	sf::Font font;
	sf::Text text;
};