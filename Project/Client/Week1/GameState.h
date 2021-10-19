#pragma once

//enum direction : char { left = 'l', right = 'r' };

enum class State { MENU, LEVEL, END };

class GameState
{
public:
	//GameState();

	void setCurrentState(State s);
	State getCurrentState();

	enum GState { MENU, LEVEL, END };

protected:
	State currentState;
};