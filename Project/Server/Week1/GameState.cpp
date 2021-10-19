#include "GameState.h"

void GameState::setCurrentState(State s)
{
	currentState = s;
}

State GameState::getCurrentState()
{
	return currentState;
}