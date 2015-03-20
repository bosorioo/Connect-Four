#include "Game.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

Game::BoardSlot::BoardSlot() // One position of the board constructor
{
	owner_player = 0;
	victory = false;
}

Game::Game(unsigned width, unsigned height)
{
	board_width = width;
	board_height = height;
	player1 = player2 = 0;
}

std::string Game::getPlayerName(int i)
{
	if (i == 1)
		return player1 ? player1->getName() : "Human Player";
	else if (i == 2)
		return player2 ? player2->getName() : "Human Player";

	return "?";
}

unsigned Game::getBoardWidth()
{
	return board_width;
}

unsigned Game::getBoardHeight()
{
	return board_height;
}

void Game::startNewGame()
{
	board.clear();
	board.resize(board_height, std::vector<Game::BoardSlot>(board_width));
	player_turn = 1;
	winner = 0;
	tied = false;
	last_col_played = -1;
}

void Game::setPlayers(Player* p1, Player* p2)
{
	player1 = p1;
	player2 = p2;

	if (p1) p1->setId(1);
	if (p2) p2->setId(-1);
}

int Game::getPlayerTurn()
{
	return player_turn;
}

int Game::getWinner()
{
	return winner;
}

bool Game::hasWinner()
{
	return winner != 0;
}

bool Game::hasTied()
{
	return tied;
}

bool Game::isHumanPlaying()
{
	return (player_turn == 1 && player1 == NULL) ||
		(player_turn == -1 && player2 == NULL);
}

std::vector<Game::BoardSlot> Game::operator[](unsigned index)
{
	// If an invalid index is given, return an empty vector
	if (index >= board.size())
		return std::vector<BoardSlot>();

	return board[index];
}

int Game::getPlayerFromPosition(unsigned row, unsigned col, bool* isVictory)
{
	if (row >= board_height || col >= board_width)
		return 0;

	if (isVictory)
		*isVictory = board[row][col].victory;

	return board[row][col].owner_player;
}

int Game::checkWinner(unsigned row, unsigned col)
{
	int player = board[row][col].owner_player;

#define CHECKLINE(r, c) if (checkLine(player, row, col, r, c) >= 4) {markLine(player, row, col, r, c); return player;}

	CHECKLINE(1, 0);
	CHECKLINE(0, 1);
	CHECKLINE(1, 1);
	CHECKLINE(1, -1);

	return 0;
}

bool Game::canPlay(int player, unsigned col)
{
	if (col >= board_width || winner != 0)
		return false;

	for (unsigned i = 0; i < board_height; i++)
		if (board[board_height - 1 - i][col].owner_player == 0)
			return player == player_turn;

	return false;
}

void Game::play(unsigned col)
{
	if (hasWinner() || hasTied() || col >= board_width)
		return;

	for (unsigned i = 0; i < board_height; i++)
	{
		if (board[board_height - 1 - i][col].owner_player == 0)
		{
			board[board_height - 1 - i][col].owner_player = player_turn;

			if (checkWinner(board_height - 1 - i, col))
				winner = player_turn;
			else
				checkTie();

			break;
		}
	}

	setNextPlayerTurn();
	last_col_played = (int)col;
}

void Game::unplay(unsigned col)
{
    if (col >= board_width)
        return;

	winner = 0;
    tied = false;
    setNextPlayerTurn();

	for (unsigned i = 0; i < board_height; i++)
	{
		if (board[i][col].owner_player == player_turn)
		{
			board[i][col].owner_player = 0;
			return;
		}
	}

	setNextPlayerTurn();
}

unsigned Game::getRandomPlay()
{
	std::vector<unsigned> cols;

	for (unsigned i = 0; i < board_width; i++)
		if (canPlay(player_turn, i))
			cols.push_back(i);

	if (cols.empty())
		return 0;

	return cols[rand() % cols.size()];
}

void Game::update(float inteval)
{
	if (!isHumanPlaying())
	{
		unsigned nextMove = 0xFFFFFFFF;

		if (player_turn == 1)
		{
			player1->setId(1);
			nextMove = player1->getNextPlay(*this);
		}
		else if (player_turn == -1)
		{
			player2->setId(-1);
			nextMove = player2->getNextPlay(*this);
		}

		if (!canPlay(player_turn, nextMove))
			nextMove = getRandomPlay();

		play(nextMove);
	}
}

void Game::print(std::string extra, std::string after)
{
	std::cout << extra;
	std::cout << " -------\n";
	for (unsigned row = 0; row < board_height; row++)
	{
		std::cout << "|";
		for (unsigned col = 0; col < board_width; col++)
		{
			std::cout << (board[row][col].owner_player == 0 ? '0' : (board[row][col].owner_player == 1 ? '1' : '2'));
		}
		std::cout << "|\n";
	}
	std::cout << " -------\n";
	std::cout << after;
}

void Game::export_as_file(std::string filepath, std::string mode, std::string extra)
{
	FILE* f = fopen(filepath.c_str(), mode.c_str());

	if (!f)
		return;

	fprintf(f, "%s\n", extra.c_str());

	for (unsigned row = 0; row < board_height; row++)
	{
		for (unsigned col = 0; col < board_width; col++)
		{
			fprintf(f, "%c", board[row][col].owner_player == 0 ? ' ' : (board[row][col].owner_player == 1 ? '1' : '2'));
		}
		fprintf(f, "\n");
	}
	fprintf(f, "----------------------------\n");
	fclose(f);
}

void Game::import_from_file(std::string filepath)
{
	FILE* f = fopen(filepath.c_str(), "r");

	if (!f)
		return;

	startNewGame();
    char c;

	for (unsigned row = 0; row < board_height; row++)
	{
		for (unsigned col = 0; col < board_width; col++)
		{
			fscanf(f, "%c", &c);

			if (c == '2')
				board[row][col].owner_player = -1;
			else if (c == '1')
				board[row][col].owner_player = 1;
			else
				board[row][col].owner_player = 0;
		}
		fscanf(f, "\n");
	}
	fclose(f);
}

void Game::import_from_str(std::string str)
{
	startNewGame();
	unsigned pos = 0;
	for (unsigned row = 0; row < board_height; row++)
	{
		for (unsigned col = 0; col < board_width; col++)
		{
			if (str.size() <= pos)
				return;

			board[row][col].owner_player = str[pos++];

			if (board[row][col].owner_player == '2')
				board[row][col].owner_player = -1;
			else if (board[row][col].owner_player == '1')
				board[row][col].owner_player = 1;
			else
				board[row][col].owner_player = 0;
		}
	}
}

int Game::checkLine(int player, unsigned row, unsigned col, int x, int y)
{
	int connected = 1;

	for (unsigned i = 1; i < 4; i++)
	{
		if (getPlayerFromPosition(row + x * i, col + y * i) != player || connected >= 4)
			break;
		else
			connected++;
	}

	for (unsigned i = 1; i < 4; i++)
	{
		if (getPlayerFromPosition(row - x * i, col - y * i) != player || connected >= 4)
			break;
		else
			connected++;
	}

	return connected;
}

void Game::markLine(int player, unsigned row, unsigned col, int x, int y)
{
	if (!player)
		return;

	if (getPlayerFromPosition(row, col) == player)
			board[row][col].victory = true;

	for (unsigned i = 1; i < 4; i++)
	{
		if (getPlayerFromPosition(row + x * i, col + y * i) == player)
			board[row + x * i][col + y * i].victory = true;

		if (getPlayerFromPosition(row - x * i, col - y * i) == player)
			board[row - x * i][ col - y * i].victory = true;
	}
}

void Game::setNextPlayerTurn()
{
	if (player_turn == 0)
		player_turn = 1;
	else
		player_turn *= -1;
}

void Game::checkTie()
{
	if (hasWinner())
		return;

	bool full = true;

	for (unsigned i = 0; i < board_width; i++)
	{
		if (board[0][i].owner_player == 0)
		{
			full = false;
			break;
		}
	}

	if (full)
		tied = true;
}
