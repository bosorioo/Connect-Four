#include "MinmaxStrategy.h"
#include <climits>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#define MAXVALUE(a, b) ((a) > (b) ? (a) : (b))
#define MINVALUE(a, b) ((a) < (b) ? (a) : (b))

MinMaxStrategy::MinMaxStrategy(unsigned deepness)
{
	max_deep = deepness;
}

unsigned MinMaxStrategy::getNextPlay(Game current)
{
	unsigned nextMove;
	int v = minmax_procedure(current, &nextMove, 0);
	return nextMove;
}

std::string MinMaxStrategy::getName()
{
	char buffer[256];
	sprintf(buffer, "MinMax (%u)", max_deep);
	return std::string(buffer);
}

int MinMaxStrategy::minmax_procedure(Game node, unsigned* nextMove, unsigned deepness)
{
	if (node.hasWinner())
		return node.getWinner() == player_id ? 1 : -1;

	if (node.hasTied() || deepness >= max_deep)
		return 0;

	int currentPlayer = node.getPlayerTurn();
	int value, bestValue;
	Game child;

	if (currentPlayer == player_id) // Maximize
	{
		bestValue = INT_MIN;
		for (unsigned i = 0; i < node.getBoardWidth(); i++)
		{
			if (node.canPlay(currentPlayer, i))
			{
				child = node;
				child.play(i);
				value = minmax_procedure(child, 0, deepness + 1);
				

				if (value > bestValue || child.getWinner() == currentPlayer ||
					(value == bestValue && rand() % 2))
				{
					bestValue = value;
					if (nextMove) *nextMove = i;
				}
			}
		}
	}
	else // Minimize
	{
		bestValue = INT_MAX;
		for (unsigned i = 0; i < node.getBoardWidth(); i++)
		{
			if (node.canPlay(currentPlayer, i))
			{
				child = node;
				child.play(i);
				value = minmax_procedure(child, 0, deepness + 1);

				if (value < bestValue || child.getWinner() == currentPlayer ||
					(value == bestValue && rand() % 2))
				{
					bestValue = value;
					if (nextMove) *nextMove = i;
				}
			}
		}
	}

	return bestValue;
}

int MinMaxStrategy::heuristic(Game node)
{
	return 0;
}