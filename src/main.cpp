#include "Game.h"
#include "Drawing.h"
#include "glutInclude.h"
#include "MinmaxStrategy.h"
#include "RandomStrategy.h"
#include "MonteCarloStrategy.h"
#include "Test.h"
#include <string>
#include <iostream>
#include <ctime>

#define UPDATE_INTERVAL_MS 20
#define CPU_THINK_INTERVAL 500
#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

#define HORIZONTAL_MARGIN 20
#define VERTICAL_MARGIN 20
#define TOP_MARGIN 160

#define MINVALUE(a, b) ((a) < (b) ? (a) : (b))

int gWindowHeight, gWindowWidth;
int gPlayerMouseColumn;

void setup();
void drawScene();
void resize(int w, int h);
void keyInput(unsigned char key, int x, int y);
void mouseInput(int button, int state, int x, int y);
void mouseMove(int x, int y);
void mouseEnter(int state);
void timerFunction(int arg);

Game game(BOARD_WIDTH, BOARD_HEIGHT);

void drawInfo();
void drawGrid();
void drawPlayerMove();
void drawGridMarks();

std::vector<Player*> players;
unsigned playerOne = 0, playerTwo = 0;
bool cpuPlaying = false;
float cpuThinkTime;

int main(int argc, char** argv)
{
	srand(time(NULL));

	if (argc > 1 && std::string(argv[1]) == "test")
		test();
	else
		std::cout << "The argument \"test\" can be passed as argument to run some tests on startup.\n";

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); 

	glutInitWindowSize (500, 600);
	glutInitWindowPosition (500, 200);
	glutCreateWindow ("Connect Four");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseInput);
	glutEntryFunc(mouseEnter);
	glutMotionFunc(mouseMove);
	glutTimerFunc(UPDATE_INTERVAL_MS, timerFunction, UPDATE_INTERVAL_MS);

	setup();
	players.push_back(0);
	players.push_back(new MinMaxStrategy(5));
	players.push_back(new MonteCarloStrategy(100));
	players.push_back(new MonteCarloStrategy(5000));
	players.push_back(new RandomStrategy);
	game.setPlayers(0, 0);
	game.startNewGame();
	glutMainLoop();

	return 0;
}

void setup()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
}

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	drawPlayerMove();
	drawInfo();
	drawGrid();
	drawGridMarks();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	gWindowHeight = h;
	gWindowWidth = w;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, 0, 1);
	glMatrixMode(GL_MODELVIEW);
}

void keyInput(unsigned char key, int x, int y)
{
	if (key == 27) // esc
	{
		game.startNewGame();
	}
	else if (key == '1')
	{
		playerOne = (playerOne + 1) % players.size();
		game.setPlayers(players[playerOne], players[playerTwo]);
		cpuThinkTime = -CPU_THINK_INTERVAL;
	}
	else if (key == '2')
	{
		playerTwo = (playerTwo + 1) % players.size();
		game.setPlayers(players[playerOne], players[playerTwo]);
		cpuThinkTime = -CPU_THINK_INTERVAL;
	}
}

void mouseInput(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	if (gPlayerMouseColumn >= 0 && game.isHumanPlaying())
		game.play(gPlayerMouseColumn);
}

void mouseMove(int x, int y)
{
	float boardWidth = (gWindowWidth - HORIZONTAL_MARGIN);
	x -= HORIZONTAL_MARGIN / 2.f;
	
	if (x < 0 || x > boardWidth)
		gPlayerMouseColumn = -1;
	else
	{
		gPlayerMouseColumn = (float)x / boardWidth * BOARD_WIDTH;

		if (gPlayerMouseColumn >= BOARD_WIDTH)
			gPlayerMouseColumn = -1;
	}
}

void mouseEnter(int state)
{
	if (state == GLUT_LEFT)
		gPlayerMouseColumn = -1;
}

void timerFunction(int arg)
{

	glutTimerFunc(UPDATE_INTERVAL_MS, timerFunction, 0);

	if (game.isHumanPlaying())
		cpuPlaying = false;
	else
	{
		if (!cpuPlaying)
		{
			cpuPlaying = true;
			cpuThinkTime = 0.f;
		}
		else
			cpuThinkTime += UPDATE_INTERVAL_MS;
	}

	glutPostRedisplay();
	
	if (cpuPlaying && cpuThinkTime >= CPU_THINK_INTERVAL)
	{
		game.update(UPDATE_INTERVAL_MS / 1000);
		cpuThinkTime = 0.f;
	}
}

void drawGrid()
{
	float boardWidth = (gWindowWidth - HORIZONTAL_MARGIN);
	float boardHeight = (gWindowHeight - VERTICAL_MARGIN - TOP_MARGIN);

	glColor3f(0.f, 0.f, 0.f);
	glBegin(GL_LINES);

	for (unsigned i = 0; i <= BOARD_HEIGHT; i++)
	{
		// Horizontal Line
		glVertex3f(HORIZONTAL_MARGIN / 2.f, VERTICAL_MARGIN / 2.f + (float)i / BOARD_HEIGHT * boardHeight, 0);
		glVertex3f(HORIZONTAL_MARGIN / 2.f + boardWidth, VERTICAL_MARGIN / 2.f + (float)i / BOARD_HEIGHT * boardHeight, 0);
	}

	for (unsigned i = 0; i <= BOARD_WIDTH; i++)
	{
		// Vertical lines
		glVertex3f(HORIZONTAL_MARGIN / 2.f + (float)i / BOARD_WIDTH * boardWidth, VERTICAL_MARGIN / 2.f, 0);
		glVertex3f(HORIZONTAL_MARGIN / 2.f + (float)i / BOARD_WIDTH * boardWidth, VERTICAL_MARGIN / 2.f + boardHeight, 0);
	}

	glEnd();
}

void drawInfo()
{
	std::string text;

	if (game.hasTied())
	{
		text = "The game has tied.";
		glColor3f(0.f, 0.f, 0.f);
	}
	else if (game.getWinner() == 1)
	{
		text = "Player 1 won the game";
		glColor3f(1.f, 0.08f, 0.08f);
	}
	else if (game.getWinner() == -1)
	{
		text = "Player 2 won the game";
		glColor3f(0.08f, 0.08f, 1.f);
	}
	else if (game.getPlayerTurn() == 1)
	{
		text = "Its player 1 turn to move";
		glColor3f(1.f, 0.08f, 0.08f);
	}
	else if (game.getPlayerTurn() == -1)
	{
		text = "Its player 2 turn to move";
		glColor3f(0.08f, 0.08f, 1.f);
	}

	Draw::text(text, 10, gWindowHeight - 28);

	glColor3f(1.f, 0.08f, 0.08f);
	Draw::text("Player 1: " + game.getPlayerName(1) + " (press '1' to change player)", 10, gWindowHeight - 88);
	glColor3f(0.08f, 0.08f, 1.f);
	Draw::text("Player 2: " + game.getPlayerName(2) + " (press '2' to change player)", 10, gWindowHeight - 118);
}

void drawPlayerMove()
{
	float boardWidth = (gWindowWidth - HORIZONTAL_MARGIN);
	float boardHeight = (gWindowHeight - VERTICAL_MARGIN - TOP_MARGIN);
	float cellHalfWidth = boardWidth / BOARD_WIDTH / 2.f;
	float circleRadius = 18.f;

	if (gPlayerMouseColumn >= 0 && game.isHumanPlaying() && !game.hasWinner() && !game.hasTied())
	{
		if (game.getPlayerTurn() == 1)
			glColor3f(1.f, 0.08f, 0.08f);
		else if (game.getPlayerTurn() == -1)
			glColor3f(0.08f, 0.08f, 1.f);

		Draw::circle2d(HORIZONTAL_MARGIN / 2.f + (float)gPlayerMouseColumn / BOARD_WIDTH * boardWidth + cellHalfWidth,
			boardHeight + VERTICAL_MARGIN / 2.f + circleRadius + 5.f, circleRadius, 30);
	}
}

void drawGridMarks()
{
	float boardWidth = (gWindowWidth - HORIZONTAL_MARGIN);
	float boardHeight = (gWindowHeight - VERTICAL_MARGIN - TOP_MARGIN);
	float cellHalfHeight = boardHeight / BOARD_HEIGHT / 2.f;
	float cellHalfWidth = boardWidth / BOARD_WIDTH / 2.f;
	float radius =  MINVALUE(cellHalfHeight, cellHalfWidth) - 5.f;

	bool vic;
	int p;

	for (unsigned row = 0; row < BOARD_HEIGHT; row++)
	{
		for (unsigned col = 0; col < BOARD_WIDTH; col++)
		{
			vic = false;
			p = game.getPlayerFromPosition(BOARD_HEIGHT - row - 1, col, &vic);
			

			if (vic)
			{
				glColor3f(0.f, 1.f, 0.f);
				Draw::circle2d(HORIZONTAL_MARGIN / 2.f + (float)col / BOARD_WIDTH * boardWidth + cellHalfWidth,
					VERTICAL_MARGIN / 2.f + (float)row / BOARD_HEIGHT * boardHeight + cellHalfHeight, radius, 30);
			}

			if (p == 1)
				glColor3f(1.f, 0.08f, 0.08f);
			else if (p == -1)
				glColor3f(0.08f, 0.08f, 1.f);
			else
				continue;

			Draw::circle2d(HORIZONTAL_MARGIN / 2.f + (float)col / BOARD_WIDTH * boardWidth + cellHalfWidth,
				VERTICAL_MARGIN / 2.f + (float)row / BOARD_HEIGHT * boardHeight + cellHalfHeight, vic ? radius - 11.f : radius, 30);

		}
	}
}