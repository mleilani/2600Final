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
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    char *payload = (char *)msg->payload;
    int keypad_data = atoi(payload);

    switch (keypad_data) {
        case '1':
            board[0][0] = 2;
            break;
        case '2':
            board[0][1] = 2;
            break;
        case '3':
            board[0][2] = 2;
            break;
        case '4':
            board[1][0] = 2;
            break;
        case '5':
            board[1][1] = 2;
            break;
        case '6':
            board[1][2] = 2;
            break;
        case '7':
            board[2][0] = 2;
            break;
        case '8':
            board[2][1] = 2;
            break;
        case '9':
            board[2][2] = 2;
            break;
        case '*':
            reset();
            break;
        default:
            break;
    }
}
//main function
int main() {
    int rc;

    mosquitto_lib_init();

    struct mosquitto *mosq = mosquitto_new("subscribe-test", true, NULL);

    mosquitto_connect_callback_set(mosq, on_connect);

    rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 10);

    if (rc) {
        printf("Could not connect to Broker with return code %d\n", rc);
        return -1;
    }

    displayMenu();

    if (menuInput == 2)
        computer = true;
    else if (menuInput == 3) {
        while (!gameOver) {
            delay(2);
            computerInput(1);
            isGameOver(1);
            drawCheck();

            if (!gameOver) {
                delay(2);
                computerInput(2);
                isGameOver(2);
                drawCheck();
            }
        }
    }

    while (!gameOver) {
        getPlayerTurn(1);
        isGameOver(1);
        drawCheck();
        if (computer && !gameOver) {
            computerInput(2);
            isGameOver(2);
            drawCheck();
        } else if (!gameOver) {
            printf("Player 2: make your move\n");
            bool waiting = true;

            while (mosq) {
                mosquitto_loop(mosq, -1, 1);
            }
            isGameOver(2);
            drawCheck();
        }
    }

    mosquitto_lib_cleanup();

    return EXIT_SUCCESS;
}

//board display
void printBoard() {
    for (int i = 0; i < 3; i++) {
        printf("+-----------+\n");
        for (int j = 0; j < 3; j++) {
            printf(" | ");

            if (board[i][j] == 1)
                printf("X");
            else if (board[i][j] == 2)
                printf("O");
            else
                printf(" ");
        }
        printf(" |\n");
    }
    printf("+-----------+\n");
}

//check for win condition

//reset
void reset() {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            board[i][j] = 0;
        }
    }
}

//validity
int isValid(int x, int y) {
    if ((x < 4 && x > 0) && (y < 4 && y > 0)) {
        int tempX = x - 1;
        int tempY = y - 1;
        if (board[tempX][tempY] == 0)
            printf("\nGood!\n");
        else {
            printf("\nInvalid Input Please try again.\n");
            return 0;
        }
        return 1;
    } else
        return 0;
}


