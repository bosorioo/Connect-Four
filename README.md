# Connect-Four
Implementation of the game Connect Four using OpenGL, with two AI algorithms to play against.  
This program uses OpenGL and GLUT for rendering.  

# Compiling (Windows)
To compile, just run the following command:  
```
g++ -o ConnectFour.exe src/Drawing.cpp src/Game.cpp src/MinmaxStrategy.cpp src/MonteCarloStrategy.cpp src/RandomStrategy.cpp src/Test.cpp src/main.cpp -Iinclude -lopengl32 -lglut32
```
You need GLUT library for the compilation to succeed.

# How to play
Use the mouse to place the piece on one of the collunms.  
Also, the following keyboard inputs can be pressed:  
* ESC: restarts the game.
* 1: Changes player 1 controller (note 1).
* 2: Changes player 2 controller (note 1).  
Note 1: Controller is who is going to make the decision for the player, defaulted to "Human Player" (waits for user input).
Controller can also be:  
* "MinMax (5)" (an AI implementation of the Min-Max algorithm with max. deep of 5, pretty silly and the slowest).
* "MonteCarlo (100)" (an AI implementation of the Monte Carlo algorithm with 100 as number of playouts before making the move).
* "MonteCarlo (5000)" (Monte Carlo algorithm with 5000 playouts, slower than the 100 version and in theory the most difficult).
