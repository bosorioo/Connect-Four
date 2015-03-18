#ifndef _PLAYER_H
#define _PLAYER_H

class Player;

#include "Game.h"
#include <string>

class Player
{
public:
	virtual unsigned getNextPlay(Game current) = 0;
	virtual std::string getName() {return "?";}
	void setId(int id) {player_id = id;}
protected:
	int player_id;
};

#endif