#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <ctype.h>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "Functions.h"

// code for color
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


// global variables for functions
char uncoveredSquare = '?';
char bomb = '*';
char flag = '^';
string commands[2] = {"flag", "uncover"};


// instructions for how to play the game
void instructions () {
    cout << "\t\t\t\t\t\t************" << endl;
    cout << "\t\t\t\t\t\tINSTRUCTIONS" << endl;
    cout << "\t\t\t\t\t\t************" << endl << endl;
    
    cout << "Before you play, you will be prompted to select a level of difficulty (easy, medium or hard)" << endl;
    cout << "The objective of the game is to uncover all squares that are not bombs" << endl;
    cout << "Should you uncover a bomb, the game will end automatically" << endl << endl;
    
    cout << "To uncover a square, type the word 'uncover' followed by the location of the square (i.e. the column letter followed by the row number)" << endl;
    cout << "For example, you may enter: uncover B 5" << endl;
    cout << "To flag a square, type the word 'flag' followed by the location of the square (i.e. the column letter followed by the row number)" << endl;
    cout << "For example, you may enter: flag B 5" << endl;
    cout << "To unflag a square, simply flag that sqaure again" << endl << endl;
    
    cout << "You win the game when all squares that are not bombs have been uncovered" << endl;
    cout << "Good luck!" << endl << endl;
}


// selecting difficulty level + dynamically allocate board + initialize all variables
void selectDifficulty(int sizes [],  char** & board, int & numRows, int & numCols, int & numBombs) {
    bool difficultySelected = false;
    string difficulty;
    while (!difficultySelected) {
        string line;
        cout << "Enter the difficulty (easy, medium or hard): ";
        getline (cin, line);
        stringstream lineStream (line);
        lineStream >> difficulty;
        
        for (int i = 0; i < (int) difficulty.length(); i++)
            difficulty[i] = tolower (difficulty[i]);
            
        if (difficulty == "easy" || difficulty == "medium" || difficulty == "hard")
            difficultySelected = true;
        else
            cout << "Error: invalid input. Please try again" << endl;
    }
    
    // set variables and dynamically allocate board according to difficulty
    if (difficulty == "easy") {
        board = new char* [sizes[0]];
        for (int i = 0; i < sizes[0]; i++)
            board[i] = new char [sizes[1]];
        
        numRows = sizes[0];
        numCols = sizes[1];
        numBombs = 10;
    }
    else if (difficulty == "medium") {
        board = new char* [sizes[2]];
        for (int i = 0; i < sizes[2]; i++)
            board[i] = new char [sizes[3]];
            
        numRows = sizes[2];
        numCols = sizes[3];
        numBombs = 40;
    }
    else {
        board = new char* [sizes[4]];
        for (int i = 0; i < sizes[4]; i++)
            board[i] = new char [sizes[5]];
            
        numRows = sizes[4];
        numCols = sizes[5];
        numBombs = 99;
    }
}


// initialize board
void initializeBoard(char** array, int numRows, int numCols) {
    for (int i = 0; i < numRows; i++)
        for (int j = 0; j < numCols; j++)
            array[i][j] = uncoveredSquare;
}


// print board
void printBoard(char** array, int numRows, int numCols) {
    cout << endl;
    for (int i = 0; i < numCols; i++)
        cout << "   " << (char) (i + 65);
    cout << endl;
    
    for (int i = 0; i < numRows; i++) {
        SetConsoleTextAttribute(hConsole, 15); // set default color (white)
        cout << "  ";
        SetConsoleTextAttribute(hConsole, 16*8); // set color (black on gray)
        for (int i = 0; i < numCols; i++)
            cout << "----";
        cout << "-" << endl;
        
        SetConsoleTextAttribute(hConsole, 15); // set default color (white)
        cout << i+1;
        if (i < 9)
            cout << " ";
        for (int j = 0; j < numCols; j++) {
            SetConsoleTextAttribute(hConsole, 16*8); // set color (black on gray)
            cout << "| ";
            printBoardCharacter (array[i][j]);
            cout << " ";
        }
        cout << "|" << endl;
    }
    
    SetConsoleTextAttribute(hConsole, 15); // set default color (white)
    cout << "  ";
    SetConsoleTextAttribute(hConsole, 16*8); // set color (black on gray)
    for (int i = 0; i < numCols; i++)
        cout << "----";
    cout << "-" << endl << endl;
    
    SetConsoleTextAttribute(hConsole, 15); // set default color (white)
}


// print board character
void printBoardCharacter (char character) {
    SetConsoleTextAttribute(hConsole, 16*8); // set default color (black on gray)
    
    if (character == '1') {
        SetConsoleTextAttribute(hConsole, 1 + 16*7); // set appropriate color (blue)
        cout << character;
    }
    else if (character == '2') {
        SetConsoleTextAttribute(hConsole, 10 + 16*7); // set appropriate color (green)
        cout << character;
    }
    else if (character == '3') {
        SetConsoleTextAttribute(hConsole, 4 + 16*7); // set appropriate color (red)
        cout << character;
    }
     else if (character == '4') {
        SetConsoleTextAttribute(hConsole, 13 + 16*7); // set appropriate color (purple)
        cout << character;
    }
    else if (character == '5') {
        SetConsoleTextAttribute(hConsole, 12 + 16*7); // set appropriate color (brownish red)
        cout << character;
    }
    else if (character > 48 && character < 57) { // all remaining numbers are black
        SetConsoleTextAttribute(hConsole, 0 + 16*7); // set appropriate color (black)
        cout << character;
    }
    else if (character == flag) {
        SetConsoleTextAttribute(hConsole, 4 + 16*8); // set appropriate color (red)
        cout << character;
    }
    else
        cout << character;
        
    SetConsoleTextAttribute(hConsole, 16 * 8); // set color back to default (black on gray)
}


// player move
void playerMove (char** board, char** boardCopy, int numRows, int numCols, int numBombs, bool & firstUncoverDone, int & numFlags) {    
    string line;
    string command;
    char col;
    int row;
    
    bool moveObtained = false;
    while (moveObtained == false) {
        cout << ">";
        getline (cin, line);
        stringstream lineStream (line);
        lineStream >> command >> col >> row;
        
        col = tolower(col); // turn col char to lowercase
        for (int i = 0; i < (int) command.length(); i++) // turn command string to lowercase
            command[i] = tolower(command[i]);
        
        if (lineStream.fail())
            cout << "Error: invalid input. Please try again." << endl;
        else if (row < 1 || row > numRows || col < 'a' || col > 97 + numCols - 1)
            cout << "Error in position. Please try again." << endl;
        else if (command != commands[0] && command != commands[1])
            cout << "Error in command. Please try again." << endl;
        else if (board[row - 1][col - 97] == flag && command == commands[1])
            cout << "Warning: you've flagged this square as a potential bomb. Please try again." << endl;
        else if (board[row - 1][col - 97] != uncoveredSquare && board[row - 1][col - 97] != flag)
            cout << "Error: you've already uncovered this square. Please try again." << endl;
        else
            moveObtained = true;
    }
    
    int colNum = col - 97;
    int rowNum = row - 1;
    
    // no error, execute player move
    if (command == commands[0]) { // player wishes to flag/unflag position
        if (board[rowNum][colNum] != flag) {
            board[rowNum][colNum] = flag;
            numFlags ++;
        }
        else {
            board[rowNum][colNum] = uncoveredSquare; // return square back to uncovered square symbol
            numFlags --;
        }
    }
    else { // player wishes to uncover position
        if (firstUncoverDone == false) {
            createBoard (boardCopy, numRows, numCols, numBombs, rowNum, colNum);
            
            firstUncoverDone = true;
        }
        
        board[rowNum][colNum] = boardCopy[rowNum][colNum];
        
        // uncover additional squares if player uncovers a blank square
        bool uncoveringFinished = false;
        while (!uncoveringFinished) {
            bool done = true;
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    if (board[i][j] == ' ') {
                        for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
                            for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
                                if (positionWithinBounds(i + deltaRow, j + deltaCol, numRows, numCols) && (deltaRow != 0 || deltaCol != 0)) { // no need to check square itself
                                    if (board[i + deltaRow][j + deltaCol] == uncoveredSquare) {
                                        board[i + deltaRow][j + deltaCol] = boardCopy[i + deltaRow][j + deltaCol];
                                        done = false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            // check if all squares that should be uncovered are uncovered
            if (done)
                uncoveringFinished = true;
        }
    }
}


// create board
void createBoard (char** boardCopy, int numRows, int numCols, int numBombs, int firstRowNum, int firstColNum) {
    // time manipulation for random
    time_t t;
    srand((unsigned) time(&t));
    
    // populate board with bombs
    for (int i = 0; i < numBombs; i++) {
        bool bombPositioned = false;
        while (!bombPositioned) {
            int row = rand() % numRows;
            int col = rand() % numCols;
            
            // ensure position is empty + position is not one of the surrounding squares of player's first move
            if (boardCopy[row][col] == ' ' && row != firstRowNum - 1 && row != firstRowNum && row != firstRowNum + 1 && col != firstColNum - 1 && col != firstColNum && row != firstColNum + 1) {
                bombPositioned = true;
                boardCopy[row][col] = bomb;
            }
        }
    }
    
    // populate board with numbers
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (boardCopy[i][j] == ' ') {
                int bombCount = 0; // total number of bombs surrounding square
                for (int deltaRow = -1; deltaRow <= 1; deltaRow++) { // check all surrounding squares to locate bombs
                    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
                        if (deltaRow != 0 || deltaCol != 0) { // eliminate unnecessary position for checking
                            if (positionWithinBounds(i + deltaRow, j + deltaCol, numRows, numCols) && boardCopy[i+deltaRow][j+deltaCol] == bomb)
                                bombCount++;
                        }
                    }
                }
                
                if (bombCount > 0)
                    boardCopy[i][j] = (bombCount + 48);
            }
        }
    }
}


// check if a position is within bounds
bool positionWithinBounds (int row, int col, int numRows, int numCols) {
    if (row < 0 || row >= numRows || col < 0 || col >= numCols)
        return false;
        
    return true;
}


// check if game is over
bool checkGameOver (char** board, char** boardCopy, int numRows, int numCols) {
    for (int i = 0; i < numRows; i++) { // bomb was uncovered
        for (int j = 0; j < numCols; j++) {
            if (board[i][j] == bomb) {
                cout << "Bomb detonated! Game over. The bombs were placed as follows:" << endl;
                return true;
            }
        }
    }
    
    for (int i = 0; i < numRows; i++) { // at least one uncovered square that isn't a bomb has not yet been uncovered
        for (int j = 0; j < numCols; j++) {
            if ((board[i][j] == uncoveredSquare || board[i][j] == flag) && boardCopy[i][j] != bomb) {
                return false;
            }
        }
    }
    
    cout << "Congratulations! You win! The bombs were placed as follows:" << endl; // if above conditions are not satisfied, player has won the game
    return true;
}