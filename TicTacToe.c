//include headers
#include <mosquitto.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//define rows / cols
#define ROW 3
#define COL 3

int menuInput;
//define integers

//string matrix
int gameBoard[ROW][COL];
bool gameOver = false;
bool computer = false;
//turns
int totalTurns = 0;
//methods
void displayMenu();
void printBoard();
//player input
void getPlayerInp(int pT);
void compInp(int player);
//game state
void drawCheck();
int isValid(int x, int y);
bool isGameEnd(int player);
//helpers
void reset();
void delay(int num_of_seconds);

//mosq keypad function

//main function

//check for win condition

//reset

//validity

