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
void createBoard();
//player input
void getPlayerInp(int p);
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
            compInp(1);
            isGameOver(1);
            drawCheck();

            if (!gameOver) {
                delay(2);
                compInp(2);
                isGameOver(2);
                drawCheck();
            }
        }
    }

    while (!gameOver) {
        playerInp(1);
        isGameOver(1);
        drawCheck();
        if (computer && !gameOver) {
            compInp(2);
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

//player inp
void playerInp(int p) {
    createBoard();

    int row = 0;
    int col = 0;

    printf("Player %d: make your move\n", p);
    scanf("%d %d", &row, &col);
    if (isValid(row, col) == 1) {
        row -= 1;
        col -= 1;
        board[row][col] = p;
        maxTurns++;
    } else
        playeInp(p);
}

bool moveInput()
{
    int temp;
    waiting = true;
    printf("Player2: make your move:\n");
    if (received)
    {
        fflush(stdin);
        result = mosquitto_publish(mosq, NULL, "state/move", 2, move, 0, false);
        return (result == MOSQ_ERR_SUCCESS);
    }
    return false;
}

bool drawCheck() {
    if (maxTurns > 9 && !gameOver) {
        printf("Draw!\n");
        gameOver = true;
        return true;
    }
    return false;
}


//comp inp
void compInp(int player) {
    createBoard();
    bool isValid = false;

    while (!isValid) {
        int row = (rand() % (3)) + 1;
        int column = (rand() % (3)) + 1;

        if (board[row - 1][column - 1] == 0) {
            board[row - 1][column - 1] = player;
            printf("Computer: makes their move\n%d %d\n", row, column);
            isValid = true;
        }
    }
    maxTurns++;
}

//menu display
void displayMenu() {
    printf("\n======================\n");
    printf("WELCOME TO TIC-TAC-TOE!\n");
    printf("1. person v person\n");
    printf("2. person v comp\n");
    printf("3. comp v comp\n");

    printf("Type out a choice and press Enter!\n");
    printf("=======================\n");
    scanf("%d", &menuInput);
    printf("\nYou have entered choice %d\n", menuInput);

    if (menuInput > 3) {
        printf("\nInvalid Choice Please Choose Again\n");
        displayMenu();
    }
}
//board display
void createBoard() {

    printf("+---------------+\n");
    printf("| %c | %c | %c | \n", board[0][0], board[0][1], board[0][2]);
    printf("+---------------+\n");
    printf("| %c | %c | %c | \n", board[1][0], board[1][1], board[1][2]);
    printf("+---------------+\n");
    printf("| %c | %c | %c | \n", board[2][0], board[2][1], board[2][2]);
    printf("+---------------+\n");  
}

//check for win condition
bool isGameOver(int player) {
    // check vertical
    for (int i = 0; i < COL; i++) {
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
            gameOver = true;
        }
    }

    // check horizontal
    for (int i = 0; i < ROW; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            gameOver = true;
        }
    }

    // check diagonal win (top left to bottom right)
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        gameOver = true;
    }

    // check diagonal win (bottom left to top right)
    if (board[2][0] == player && board[1][1] == player && board[0][2] == player) {
        gameOver = true;
    }

    if (gameOver) {
        if (computer && player == 2) {
            printf("Game over! Result: Computer wins!\n");
            createBoard();
        } else {
            printf("Player %d Wins!\n", player);
            createBoard();
        }
    }
    return gameOver;
}
//gamereset

void delay(int number_of_seconds) {
    int milli_seconds = 1000 * number_of_seconds;

    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds)
        ;
}

//reset board
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

bool connect() {
    mosq = mosquitto_new(NULL, true, NULL);

    int rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 30);

    if (rc != MOSQ_ERR_SUCCESS) {
        printf("Error connecting");
        return false;
    }

    rc = mosquitto_subscribe(mosq, NULL, "ESP32/input", 0);

    if (rc != MOSQ_ERR_SUCCESS) {
        printf("Could not connect to broker with return code %d\n", rc);
        return false;
    }

    return true;
}
