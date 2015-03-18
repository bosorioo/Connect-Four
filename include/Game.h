#ifndef _GAME_H
#define _GAME_H

class Game;

#include <vector>
#include "Player.h"

class Game
{
public:

	struct BoardSlot {
		BoardSlot();
		bool victory;
		int owner_player;
	};

	Game(unsigned width = 7, unsigned height = 6);
	std::string getPlayerName(int i);
	unsigned getBoardWidth();
	unsigned getBoardHeight();
	void startNewGame();
	void setPlayers(Player* p1 = 0, Player* p2 = 0);
	int getPlayerTurn();
	int getWinner();
	bool hasWinner();
	bool hasTied();
	bool isHumanPlaying();
	std::vector<BoardSlot> operator [] (unsigned index);
	int getPlayerFromPosition(unsigned row, unsigned col, bool* isVictory = 0);
	int checkWinner(unsigned row, unsigned col);
	bool canPlay(int player, unsigned col);
	void play(unsigned col);
	unsigned getRandomPlay();

	void update(float inteval);
	void print(std::string extra = "", std::string after = "");
	void export_as_file(std::string filepath, std::string mode = "w", std::string extra = "");
	void import_from_file(std::string filepath);
	void import_from_str(std::string str);

	void setNextPlayerTurn();

private:

	int checkLine(int player, unsigned row, unsigned col, int x, int y);
	void markLine(int player, unsigned row, unsigned col, int x, int y);
	void checkTie();

	std::vector< std::vector<BoardSlot> > board;
	unsigned board_width, board_height;
	int player_turn;
	int winner;
	bool tied;

	Player *player1, *player2;

};

#endif