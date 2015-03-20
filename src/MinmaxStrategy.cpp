#include "MinmaxStrategy.h"
#include <climits>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

MinMaxStrategy::MinMaxStrategy(unsigned deepness)
{
	max_deep = deepness;
}

unsigned MinMaxStrategy::getNextPlay(Game current)
{
	unsigned nextMove;
	minmax_procedure(current, &nextMove, 0);
	//minmax_ab_procedure(current, &nextMove, 0, INT_MIN, INT_MAX);
	return nextMove;
}

std::string MinMaxStrategy::getName()
{
	char buffer[256];
	sprintf(buffer, "MinMax (%u)", max_deep);
	return std::string(buffer);
}

int MinMaxStrategy::minmax_procedure(Game& node, unsigned* nextMove, unsigned deepness)
{
	if (node.hasWinner())
		return (max_deep + 1 - deepness) * (node.getWinner() == player_id ? 1 : -1);

	if (node.hasTied() || deepness >= max_deep)
		return 0;

	int currentPlayer = node.getPlayerTurn();
	int value, bestValue;
	unsigned i;

	if (currentPlayer == player_id) // Maximize
	{
		bestValue = INT_MIN;

		for (i = 0; i < node.getBoardWidth(); i++)
		{
			if (node.canPlay(currentPlayer, i))
			{
				node.play(i);
				value = minmax_procedure(node, 0, deepness + 1);

				if (value > bestValue || node.getWinner() == currentPlayer ||
					(bestValue == value && rand() % 2))
				{
					bestValue = value;
					if (nextMove) *nextMove = i;
				}
				node.unplay(i);
			}
		}
	}
	else // Minimize
	{
		bestValue = INT_MAX;
		for (i = 0; i < node.getBoardWidth(); i++)
		{
			if (node.canPlay(currentPlayer, i))
			{
				node.play(i);
				value = minmax_procedure(node, 0, deepness + 1);

				if (value < bestValue || node.getWinner() == currentPlayer ||
					(bestValue == value && rand() % 2))
				{
					bestValue = value;
					if (nextMove) *nextMove = i;
				}

				node.unplay(i);
			}
		}
	}

	return bestValue * (max_deep + 1 - deepness);
}

int MinMaxStrategy::minmax_ab_procedure(Game& node, unsigned* nextMove, unsigned deepness, int alpha, int beta)
{
	if (node.hasWinner())
		return (max_deep + 1 - deepness) * (node.getWinner() == player_id ? 1 : -1);

	if (node.hasTied() || deepness >= max_deep)
		return 0;

	int currentPlayer = node.getPlayerTurn();
	int value;
	unsigned i;

	if (currentPlayer == player_id) // Maximize
	{
		value = INT_MIN;

		for (i = 0; i < node.getBoardWidth(); i++)
		{
			if (node.canPlay(currentPlayer, i))
			{
				node.play(i);
				value = MAX(value, minmax_ab_procedure(node, 0, deepness + 1, alpha, beta));
				alpha = MAX(alpha, value);

				if (beta <= alpha)
				{
				    node.unplay(i);
                    break;
				}

				if (value > value || node.getWinner() == currentPlayer ||
					(value == value && rand() % 2))
				{
					value = value;
					if (nextMove) *nextMove = i;
				}
				node.unplay(i);
			}
		}
	}
	else // Minimize
	{
		value = INT_MAX;
		for (i = 0; i < node.getBoardWidth(); i++)
		{
			if (node.canPlay(currentPlayer, i))
			{
				node.play(i);
				value = MIN(value, minmax_ab_procedure(node, 0, deepness + 1, alpha, beta));
				beta = MIN(beta, value);

				if (beta <= alpha)
				{
				    node.unplay(i);
                    break;
				}

				if (value < value || node.getWinner() == currentPlayer ||
					(value == value && rand() % 2))
				{
					value = value;
					if (nextMove) *nextMove = i;
				}

				node.unplay(i);
			}
		}
	}

	return value * (max_deep + 1 - deepness);
}

int MinMaxStrategy::heuristic(Game node)
{
	return 0;
}
