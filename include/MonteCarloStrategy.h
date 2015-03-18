#ifndef _MONTECARLO_ST_H
#define _MONTECARLO_ST_H

#include "Player.h"
#include "Game.h"

class MonteCarloStrategy : public Player
{
public:
	MonteCarloStrategy(unsigned strength = 10000);
	unsigned getNextPlay(Game current);
	std::string getName();
	void setStrength(unsigned strength);
	int expand(Game node);
private:
	float evaluate(Game node, unsigned col);
	unsigned playouts;
};

#endif