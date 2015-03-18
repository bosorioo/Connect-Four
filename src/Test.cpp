#include <iostream>
#include "Game.h"
#include "Player.h"
#include "MinmaxStrategy.h"
#include "RandomStrategy.h"
#include "MonteCarloStrategy.h"

#include <vector>
#include <string>

void testMinMax();
void testMatch(Player* p1, Player* p2, unsigned numOfMatches);
void testMatches();
void testProblems();

void testMinMax()
{
	std::cout << "Testing MinMax on a 4x3 board with predefined states\n";
	Game game(4, 3);
	MinMaxStrategy st;

	st.setId(-1);
	
	std::vector<std::string> strs;

	strs.push_back(
		"0000"
		"0000"
		"1102");

	strs.push_back(
		"0000"
		"1000"
		"1020");

	strs.push_back(
		"0000"
		"1010"
		"1220");

	strs.push_back(
		"0000"
		"1121"
		"1221");

	strs.push_back(
		"0000"
		"1210"
		"1122");

	for (unsigned i = 0; i < strs.size(); i++)
	{
		game.import_from_str(strs[i]);
		game.setNextPlayerTurn();
		game.print("===");
		std::cout << "Next play: " << st.getNextPlay(game) << "\n";
	}

	std::cout << "Test over.";
}

void testMatch(Player* p1, Player* p2, unsigned numOfMatches)
{
	unsigned p1Victories = 0;
	unsigned p2Victories = 0;
	unsigned ties = 0;

	Game game(7, 6);

	std::cout << "Playing " << numOfMatches << " matches of " << p1->getName() << " vs " << p2->getName() << ":\n";
	for (unsigned n = 0; n < numOfMatches; n++)
	{
		game.setPlayers(p1, p2);
		game.startNewGame();
		while (!game.hasTied() && !game.hasWinner())
			game.update(0);
		if (game.hasTied())
			ties++;
		else if (game.getWinner() == 1)
			p1Victories++;
		else
			p2Victories++;

		if (!n)
			std::cout << "Done match #" << (n+1);
		else
			std::cout << ", #" << (n + 1);
	}

	std::cout << ".\nMatches over.\nStatistics:\n\t" << p1Victories << " victories for weak.\n\t";
	std::cout << p2Victories << " victories for strong.\n\t" << ties << " ties.\n";
}

void testMatches()
{
	unsigned matches = 100;
	
	std::vector<Player*> players;
	players.push_back(new MonteCarloStrategy(100));
	players.push_back(new MonteCarloStrategy(5000));
	players.push_back(new RandomStrategy);
	players.push_back(new MinMaxStrategy(2));
	players.push_back(new MinMaxStrategy(5));

	for (unsigned i = 0; i < players.size(); i++)
	{
		for (unsigned j = i + 1; j < players.size(); j++)
		{
			testMatch(players[i], players[j], matches);
			testMatch(players[j], players[i], matches);
		}
	}
}

void testProblems()
{
	std::vector<std::string> problems;
	std::vector<Player*> players;
	players.push_back(new MonteCarloStrategy(100));
	players.push_back(new MonteCarloStrategy(50000));
	players.push_back(new MinMaxStrategy(4));
	players.push_back(new MinMaxStrategy(12));

	problems.push_back("(Problem 45, book answer is column 0)");
	problems.push_back(
		"0000000"
		"0222110"
		"0112220"
		"0221110"
		"1212210"
		"1221211");

	Game game(7, 6);

	for (unsigned i = 0; i < problems.size(); i += 2)
	{
		game.import_from_str(problems[i + 1]);
		game.print("Given the following board " + problems[i] + ":\n");
		for (unsigned j = 0; j < players.size(); j++)
		{
			players[j]->setId(1);

			if (game.getPlayerTurn() != 1)
				game.setNextPlayerTurn();

			unsigned nextPlay = players[j]->getNextPlay(game);
			std::cout << players[j]->getName() << " would play on column " << nextPlay << ".\n";
		}
		std::cout << "\n";
	}
}

void test()
{
	std::cout << "Test Unit:\n\n";

	testProblems();
	testMatches();

	std::cout << "Tests finished.\n";
	std::cin.get();
}