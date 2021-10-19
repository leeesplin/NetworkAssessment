#pragma once

//enum direction : char { left = 'l', right = 'r' };

enum class State {LEVEL};

class GameState
{
public:
	//GameState();

	void setCurrentState(State s);
	State getCurrentState();

	enum GState {LEVEL};

protected:
	State currentState;
};