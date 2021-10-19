// Week1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Level.h"
#include "Menu.h"
#include "EndScreen.h"
#include "Input.h"
#include "GameState.h"

int main()
{
	// create window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Lab 1");

	// define game objects
	Input input;
	GameState gameState;
	gameState.setCurrentState(State::MENU);	
	EndScreen endScreen(&window, &input, &gameState);
	Level level(&window, &input, &gameState, &endScreen);
	Menu menu(&window, &input, &gameState);

	// for delta time
	sf::Clock clock;
	float deltaTime;

	// game loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				window.setView(sf::View(sf::FloatRect(0, 0, (float)event.size.width, (float)event.size.height)));
				break;
			case sf::Event::KeyPressed:
				input.setKeyDown(event.key.code);
				break;
			case sf::Event::KeyReleased:
				input.setKeyUp(event.key.code);
				break;
			case sf::Event::MouseMoved:
				input.setMousePosition(event.mouseMove.x, event.mouseMove.y);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					input.setMouseLeftDown(true);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					input.setMouseRightDown(true);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					input.setMouseLeftDown(false);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					input.setMouseRightDown(false);
				}
				break;
			default:
				break;
			}
		}

		// calculate delta time. how much time has passed since it was last
		// calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();

		switch (gameState.getCurrentState())
		{
		case(State::MENU):
			// call menu screen	
			menu.handleInput(deltaTime);
			menu.update(deltaTime);
			menu.render();
		break;

		case(State::LEVEL):
			// call standard game loop functions (input, update and render)
			level.handleInput(deltaTime);
			level.update(deltaTime);
			level.render();
		break;

		case(State::END):
			// call end screen	
			endScreen.handleInput(deltaTime);
			endScreen.update(deltaTime);
			endScreen.render();
		}
	}
}