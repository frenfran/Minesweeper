#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

#include "Functions.h"

int sizes [6] = {8, 10, 14, 18, 20, 24};

int main () {
    char** board; // gameboard pointer
    char** boardCopy; // copy of gameboard pointer
    int numRows; // number of rows
    int numCols; // number of columns
    int numBombs; // number of bombs
    cout << "COLLECTION OF CLASSIC GAMES" << endl << endl << "Welcome to..." << endl << "MINESWEEPER" << endl << endl; // header
    
    // Instructions
    instructions ();
    
    // begin by selecting difficulty
    selectDifficulty (sizes, board, numRows, numCols, numBombs);
    
    // create empty game array with the same dimensions as board
    boardCopy = new char* [numRows];
        for (int i = 0; i < numRows; i++)
            boardCopy[i] = new char [numCols];
    for (int i = 0; i < numRows; i++)
        for (int j = 0; j < numCols; j++)
            boardCopy [i][j] = ' ';
    
    // initialize and print board
    initializeBoard (board, numRows, numCols);
    printBoard(board, numRows, numCols);
    
    bool gameOver = false;
    bool firstUncoverDone = false;
    int flagsSet = 0; // number of flags set
    while (!gameOver) { // gameplay
        playerMove (board, boardCopy, numRows, numCols, numBombs, firstUncoverDone, flagsSet);
        cout << endl << "Flags left: " << numBombs - flagsSet << endl; // display number of remaining bombs (flags left)   
        printBoard(board, numRows, numCols);
        
        // printBoard(boardCopy, numRows, numCols); // debugging
    
        if (checkGameOver(board, boardCopy, numRows, numCols) == true) {
            gameOver = true;
            printBoard(boardCopy, numRows, numCols);
            break;
        }
    }
}