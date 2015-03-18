#include "MonteCarloStrategy.h"
#include <limits>
#include <iostream>

MonteCarloStrategy::MonteCarloStrategy(unsigned strength)
{
	playouts = strength;
}

unsigned MonteCarloStrategy::getNextPlay(Game current)
{
	unsigned columnToPlay = 100;
	float bestValue = -std::numeric_limits<float>::max();

	for (unsigned i = 0; i < current.getBoardWidth(); i++)
	{
		if (!current.canPlay(player_id, i))
			continue;

		float currentValue = evaluate(current, i);

		if (currentValue > bestValue) // (currentValue == bestValue && rand() % 2)
		{
			bestValue = currentValue;
			columnToPlay = i;
		}
	}

	return columnToPlay;
}


std::string MonteCarloStrategy::getName()
{
	char buffer[256];
	sprintf(buffer, "MonteCarlo (%u)", playouts);
	return std::string(buffer);
}

float MonteCarloStrategy::evaluate(Game current, unsigned col)
{
	float value = 0;
	current.play(col);

	for (unsigned i = 0; i < playouts; i++)
	{
		int winner = expand(current);

		if (winner == player_id)
			value += 1.f;
		else if (winner == -player_id)
			value -= 1.f;
		// else is draw, so do nothing
	}
	return (value / (float)playouts);
}

int MonteCarloStrategy::expand(Game node)
{
	while (!node.hasTied() && !node.hasWinner())
	{
		node.play(node.getRandomPlay());
	}
	
	return node.getWinner();
}

void MonteCarloStrategy::setStrength(unsigned strength)
{
	playouts = strength;
}