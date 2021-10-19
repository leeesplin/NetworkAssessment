#include "EndScreen.h"



EndScreen::EndScreen(sf::RenderWindow* hwnd, Input* input, GameState* gameState)
{
	window = hwnd;
	inputObj = input;
	gameStateObj = gameState;

	//set background image properties
	backgroundTexBlue.loadFromFile("gfx/BlueTanks.png");
	backgroundTexRed.loadFromFile("gfx/RedTanks.png");
	backgroundImage.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	backgroundImage.setPosition(0, 0);

	//load font
	if (!font.loadFromFile("font/arial.ttf"))
	{
		std::cout << "Error loading font\n";
	}

	text.setFont(font);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::White);
	text.setPosition(260, 400);
}

EndScreen::~EndScreen()
{

}

void EndScreen::update(float deltaTime)
{
	//sets the background image based on who won
	if (winnerBackground == 0)
	{
		backgroundImage.setTexture(&backgroundTexBlue);
	}
	else
	{
		backgroundImage.setTexture(&backgroundTexRed);
	}
}

void EndScreen::handleInput(float deltaTime)
{
	//starts the level once space is pressed
	if (inputObj->isKeyDown(sf::Keyboard::Escape))
	{
		inputObj->setKeyUp(sf::Keyboard::Escape);
		window->close();
	}

	text.setString("PRESS ESCAPE TO CLOSE");
}

void EndScreen::render()
{
	beginDraw();

	window->draw(backgroundImage);
	window->draw(text);

	endDraw();
}

void EndScreen::beginDraw()
{
	window->clear(sf::Color::Black);
}

void EndScreen::endDraw()
{
	window->display();
}

void EndScreen::setBackground(int newBackground)
{
	winnerBackground = newBackground;
}
