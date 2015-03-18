#ifndef _RANDOM_ST_H
#define _RANDOM_ST_H

#include "Player.h"

class RandomStrategy : public Player
{
public:
	unsigned getNextPlay(Game current);
	std::string getName();
};

#endif