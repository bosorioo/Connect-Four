#include "RandomStrategy.h"
#include <climits>
#include <iostream>

unsigned RandomStrategy::getNextPlay(Game current)
{
	return current.getRandomPlay();
}

std::string RandomStrategy::getName()
{
	return "Random CPU";
}