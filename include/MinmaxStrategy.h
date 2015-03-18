#ifndef _MINMAX_ST_H
#define _MINMAX_ST_H

#include "Player.h"

class MinMaxStrategy : public Player
{
public:
	MinMaxStrategy(unsigned deepness = 3);
	unsigned getNextPlay(Game current);
	std::string getName();
	int minmax_procedure(Game node, unsigned * nextMove, unsigned deepness);
	int heuristic(Game node);
private:
	unsigned max_deep;
};

#endif