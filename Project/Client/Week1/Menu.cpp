#include "Menu.h"

Menu::Menu(sf::RenderWindow* hwnd, Input* input, GameState* gameState)
{
	window = hwnd;
	inputObj = input;
	gameStateObj = gameState;

	//set background image properties
	backgroundTex.loadFromFile("gfx/Tanks.png");
	backgroundImage.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	backgroundImage.setPosition(0, 0);
	backgroundImage.setTexture(&backgroundTex);

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

Menu::~Menu()
{

}

void Menu::update(float deltaTime)
{
}

void Menu::handleInput(float deltaTime)
{
	//starts the level once space is pressed
	if (inputObj->isKeyDown(sf::Keyboard::Space))
	{
		inputObj->setKeyUp(sf::Keyboard::Space);
		gameStateObj->setCurrentState(State::LEVEL);
	}

	text.setString("PRESS SPACE TO CONNECT");
}

void Menu::render()
{
	beginDraw();

	window->draw(backgroundImage);
	window->draw(text);

	endDraw();
}

void Menu::beginDraw()
{
	window->clear(sf::Color::Black);
}

void Menu::endDraw()
{
	window->display();
}
