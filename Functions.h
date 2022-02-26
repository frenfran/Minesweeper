#pragma once

#include <iostream>

// instructions
void instructions();

// selecting difficulty
// initializes values for numRows, numCols and numBombs
// dynamically allocates the game board
void selectDifficulty(int*, char** &, int &, int &, int &);

// initialize board
void initializeBoard (char**, int, int);

// print board
void printBoard (char**, int, int);

// print board character
// takes a character as a parameter and prints it using the right colour
// if a square has a number and is uncovered, background becomes lighter
// flags are red while uncovered squares are black
void printBoardCharacter (char);

// player move
void playerMove (char**, char**, int, int, int, bool &, int &);

// create the game board
void createBoard (char**, int, int, int, int, int);

// check if a position is within bounds
// returns true if position is within bounds, false if not
bool positionWithinBounds (int, int, int, int);

// check if game is over
// returns true if game is over, false if not
bool checkGameOver (char**, char**, int, int);