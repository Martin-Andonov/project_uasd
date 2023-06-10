#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BOARD_SIZE 10
#define SHIP_TYPES 4
#define MAX_SHIPS_PER_TYPE 4
#define MAX_SHIP_LENGTH 6
#define NUM_SHIPS 10
#define MAX_PLACEMENT_ATTEMPTS 50

typedef struct
{
    int x;            // x-coordinate of the starting point
    int y;            // y-coordinate of the starting point
    int length;       // length of the ship
    bool isHorizontal;// flag for the direction of the ship (true - horizontal, false - vertical)
} Ship;

typedef struct
{
    int x;
    int y;
} Coordinate;

// Ship lengths
const int shipLengths[NUM_SHIPS] = {6, 4, 4, 3, 3, 3, 2, 2, 2, 2};

char board[BOARD_SIZE][BOARD_SIZE];
Ship ships[NUM_SHIPS];

// Function to clear the game board
void clearBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = ' ';
        }
    }
}

// Function to print the game board
void printBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    printf("   ");
    for (char c = 'A'; c < 'A' + BOARD_SIZE; c++)
    {
        printf("%c ", c);
    }
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Function to check if coordinates (x, y) are valid
bool isValidCoordinates(int x, int y)
{
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

bool isHit(Coordinate guess)
{
    return board[guess.y][guess.x] == '#';
}

// Function to check if the ship placement is valid
bool isValidPlacement(Ship ship, char board[BOARD_SIZE][BOARD_SIZE])
{
    if (ship.isHorizontal)
    {
        if (!isValidCoordinates(ship.x, ship.y) ||
                !isValidCoordinates(ship.x + ship.length - 1, ship.y))
            return false;
        for (int i = 0; i < ship.length; i++)
        {
            if (board[ship.y][ship.x + i] != ' ')
            {
                return false;
            }
        }
    }
    else
    {
        if (!isValidCoordinates(ship.x, ship.y) ||
                !isValidCoordinates(ship.x, ship.y + ship.length - 1))
            return false;
        for (int i = 0; i < ship.length; i++)
        {
            if (board[ship.y + i][ship.x] != ' ')
            {
                return false;
            }
        }
    }
    return true;
}

// Function to check if the ship overlaps with already placed ships
bool isOverlap(Ship ship, char board[BOARD_SIZE][BOARD_SIZE])
{
    if (ship.isHorizontal)
    {
        for (int i = -1; i <= ship.length; i++)
        {
            if (board[ship.y][ship.x + i] != ' ' || board[ship.y-1][ship.x + i] != ' ' || board[ship.y+1][ship.x + i] != ' ')
            {
                return true;
            }
        }
    }
    else
    {
        for (int i = -1; i <= ship.length; i++)
        {
            if (board[ship.y + i][ship.x] != ' ' || board[ship.y + i][ship.x -1] != ' ' || board[ship.y + i][ship.x + 1] != ' ')
            {
                return true;
            }
        }
    }
    return false;
}




// Function to place a ship on the game board
void placeShip(Ship ship, char board[BOARD_SIZE][BOARD_SIZE])
{
    if (ship.isHorizontal)
    {
        for (int i = 0; i < ship.length; i++)
        {
            board[ship.y][ship.x + i] = '#';
        }
    }
    else
    {
        for (int i = 0; i < ship.length; i++)
        {
            board[ship.y + i][ship.x] = '#';
        }
    }
}

bool generateOrientation()
{
    return rand() % 2 == 0;  // 0 represents vertical, 1 represents horizontal
}

int generateCoordinate()
{
    return rand() % BOARD_SIZE;
}

void placeShips()
{
    clearBoard(board);

    int attempts = 0;

    for (int i = 0; i < NUM_SHIPS; i++)
    {
        Ship ship;
        ship.length = shipLengths[i];
        ship.isHorizontal = generateOrientation();

        // Generate random coordinates until a valid placement is found or maximum attempts reached
        do
        {
            ship.x = generateCoordinate();
            ship.y = generateCoordinate();

            attempts++;
            if (attempts > MAX_PLACEMENT_ATTEMPTS)
            {
                i = -1;  // Restart the placement process from the first ship
                attempts = 0;
                clearBoard(board);
                break;
            }
        } while (!isValidPlacement(ship, board) || isOverlap(ship, board));

        if (i >= 0)
        {
            placeShip(ship, board);
            ships[i] = ship;
        }
    }
}

// Function to enter the coordinates from the user
void enterCoordinates(int* x, int* y)
{
    char input[5];
    fflush(stdin);
    scanf("%s", input);
    *x = input[0] - 'A';
    if(input[1] != '1' || input[2] != '0')
    {
        *y = input[1] - '1';
    }
    else
    {
        *y = 9;
    }
}

// Function to enter the direction from the user
bool enterDirection()
{
    char input[5];
    scanf("%s", input);
    return input[0] == 'h' || input[0] == 'H';
}

// Function to enter the ship configurations from the user
void enterConfiguration(Ship ships[SHIP_TYPES][MAX_SHIPS_PER_TYPE], char board[BOARD_SIZE][BOARD_SIZE])
{
    int shipSizes[SHIP_TYPES][MAX_SHIPS_PER_TYPE] =
    {
        {2, 2, 2, 2},
        {3, 3, 3},
        {4, 4},
        {6}
    };

    int desiredShipCounts[SHIP_TYPES] = {4, 3, 2, 1};

    int currentShipIndex = 0;

    for (int i = 0; i < SHIP_TYPES; i++)
    {
        int length = shipSizes[i][0];
        int shipCount = 0;

        while (shipCount < desiredShipCounts[i])
        {
            printf("Options:\n");
            printf("1. View the current board\n");
            printf("2. Change the position of an already placed ship\n");
            printf("3. Continue\n");
            printf("Enter your choice: ");

            int choice;
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                printf("Current board:\n");
                printBoard(board);
                break;
            case 2:
                if (shipCount > 0)
                {
                    printf("Enter the ship number (1-%d) to change its position: ", shipCount);
                    int shipNumber;
                    scanf("%d", &shipNumber);

                    if (shipNumber >= 1 && shipNumber <= shipCount)
                    {
                        printf("Enter the new starting position for ship %d (length %d): ", shipNumber, length);
                        int x, y;
                        enterCoordinates(&x, &y);
                        printf("Enter the direction (h for horizontal, v for vertical): ");
                        bool isHorizontal = enterDirection();

                        Ship newShip = {x, y, length, isHorizontal};

                        if (isValidPlacement(newShip, board) && !isOverlap(newShip, board))
                        {
                            // Remove the old ship
                            Ship oldShip = ships[i][shipNumber - 1];
                            if (oldShip.isHorizontal)
                            {
                                for (int j = 0; j < oldShip.length; j++)
                                {
                                    board[oldShip.y][oldShip.x + j] = ' ';
                                }
                            }
                            else
                            {
                                for (int j = 0; j < oldShip.length; j++)
                                {
                                    board[oldShip.y + j][oldShip.x] = ' ';
                                }
                            }

                            ships[i][shipNumber - 1] = newShip;
                            placeShip(newShip, board);
                            printf("Ship %d position changed.\n", shipNumber);
                        }
                        else
                        {
                            printf("Invalid position or overlap. Please try again.\n");
                        }
                    }
                    else
                    {
                        printf("Invalid ship number. Please try again.\n");
                    }
                }
                else
                {
                    printf("No ships have been placed yet. Please choose another option.\n");
                }
                break;
            case 3:
                printf("Enter the starting position for ship %d (length %d): ", currentShipIndex + 1, length);
                int x, y;
                enterCoordinates(&x, &y);
                printf("Enter the direction (h for horizontal, v for vertical): ");
                bool isHorizontal = enterDirection();

                Ship newShip = {x, y, length, isHorizontal};

                if (isValidPlacement(newShip, board) && !isOverlap(newShip, board))
                {
                    ships[i][shipCount] = newShip;
                    placeShip(newShip, board);
                    shipCount++;
                    currentShipIndex++;
                    printf("Ship %d placed.\n", currentShipIndex);
                }
                else
                {
                    printf("Invalid position or overlap. Please try again.\n");
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    }
}


void enterConfigurationFromFile(const char* filename, Ship ships[SHIP_TYPES][MAX_SHIPS_PER_TYPE], char board[BOARD_SIZE][BOARD_SIZE])
{
    int shipSizes[SHIP_TYPES][MAX_SHIPS_PER_TYPE] = {
        {2, 2, 2, 2},
        {3, 3, 3},
        {4, 4},
        {6}
    };

    int desiredShipCounts[SHIP_TYPES] = {4, 3, 2, 1};

    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char position[3];
    char direction;

    int currentShipIndex = 0;
    int shipCount = 0;

    while (fscanf(file, "%s %c\n", position, &direction) == 2)
    {
        int x = position[0] - 'A';
        int y = 0;

        if (position[1] != '1' || position[2] != '0')
        {
            y = position[1] - '1';
        }
        else
        {
            y = 9;
        }

        int length = shipSizes[currentShipIndex][0];
        bool isHorizontal = (direction == 'h');

        Ship newShip = {x, y, length, isHorizontal};

        if (1)//isValidPlacement(newShip, board) && !isOverlap(newShip, board) has a bug
        {
            ships[currentShipIndex][shipCount] = newShip;
            placeShip(newShip, board);
            shipCount++;

            if (shipCount >= desiredShipCounts[currentShipIndex])
            {
                currentShipIndex++;
                shipCount = 0;
            }
        }
        else
        {
            printf("Invalid position or overlap in file: %s\n", filename);
            fclose(file);
            return;
        }
    }

    fclose(file);
}


Coordinate getRandomCoordinate()
{
    Coordinate coordinate;
    coordinate.x = rand() % BOARD_SIZE;
    coordinate.y = rand() % BOARD_SIZE;
    return coordinate;
}

bool isSunk(int shipIndex)
{
    Ship ship = ships[shipIndex];
    for (int i = 0; i < ship.length; i++)
    {
        if (ship.isHorizontal)
        {
            if (board[ship.y][ship.x + i] == '#')
                return false;
        }
        else
        {
            if (board[ship.y + i][ship.x] == '#')
                return false;
        }
    }
    return true;
}

void printShips()
{
    printBoard(board);

    for (int i = 0; i < NUM_SHIPS; i++)
    {
        printf("Ship %d: Length %d, Position (%d, %d), ", i + 1, ships[i].length, ships[i].x, ships[i].y);
        printf("%s\n", ships[i].isHorizontal ? "Horizontal" : "Vertical");
    }
}

void printChessCoordinates(int x, int y)
{
    char column = 'A' + x;
    int row = y + 1;
    printf("%c%d\n", column, row);
}

void writeChessCoordinatesToFile(const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    for (int i = 0; i < NUM_SHIPS; i++)
    {
        int x = ships[i].x;
        int y = ships[i].y;
        char column = 'A' + x;
        int row = y + 1;
        fprintf(file, "%c%d %c\n", column, row, ships[i].isHorizontal ? 'h' : 'v');
    }

    fclose(file);

    printf("Chess-like coordinates written to %s\n", filename);
}

int get_file_size(FILE *f) {
  fseek(f, 0, SEEK_END);
  return ftell(f);
}

int get_avarage_value_of_string(char *str) {
  int avarage = 0, i = 0;

  while (str[i] != '\0') {
    avarage += (int)str[i];
    i++;
  }

  return avarage / i;
}

char *generate_key(char *password) {
  char *key = (char *)malloc(
      sizeof(char) *
      (strlen(password) + 1)); // key will contain the password + average value
                               // of elements in password

  key[0] = (char)get_avarage_value_of_string(password);
  strcat(key, password);

  printf("\npass:%s,time:%d,key:%s,avr:%d\n", password, time(NULL) % 254 + 1,
         key, get_avarage_value_of_string(password));

  return key;
}

void encrypt() {
  FILE *fdata = fopen("replay.txt", "rb+");
  int fileSize = get_file_size(fdata);
  char *file_data = (char *)malloc(fileSize + 1);
  char *output = (char *)malloc(fileSize + 2);
  char input[100];
  char *key;
  rewind(fdata);

  printf("Enter password to encrypt:");
  scanf("%s", input);

  if (fdata == NULL) {
    printf("File can't open");
    return;
  }

  key = generate_key(input);

  output[0] = key[0];

  fread(file_data, fileSize, sizeof(char), fdata);

  for (int i = 0; i < strlen(key); i++) {
    for (int j = 0; j < fileSize; j++) {
      if (key[i] != file_data[j]) {
        file_data[j] = file_data[j] ^ key[i];
      }
    }
  }

  strcat(output, file_data);

  rewind(fdata);
  fwrite(output, fileSize + 1, sizeof(char), fdata);
  fclose(fdata);
  free(file_data);
}

void decrypt() {
  FILE *fdata = fopen("replay.txt", "rb+");
  int fileSize = get_file_size(fdata);
  char *file_data = (char *)malloc(fileSize + 1);
  char psw[100], key[100];
  rewind(fdata);

  printf("Enter password for decryption:");
  scanf("%s", psw);

  if (fdata == NULL) {
    printf("File can't open");
    return;
  }

  fread(file_data, fileSize, sizeof(char), fdata);

  key[0] = file_data[0];
  strcat(key, psw);

  for (int i = strlen(key); i >= 0; i--) {
    for (int j = 0; j < fileSize; j++) {
      if (key[i] != file_data[j]) {
        file_data[j] = file_data[j] ^ key[i];
      }
    }
  }

  rewind(fdata);
  fwrite(file_data, fileSize, sizeof(char), fdata);
  fclose(fdata);
  free(file_data);
}

// Function to update the opponent's board based on the guess
void updateOpponentBoard(int x, int y, char board[BOARD_SIZE][BOARD_SIZE], char opponentBoard[BOARD_SIZE][BOARD_SIZE])
{
    if (board[y][x] == '#')
    {
        opponentBoard[y][x] = 'X';
    }
    else
    {
        opponentBoard[y][x] = 'O';
    }
}

// Function to check if all the ships have been sunk
bool areAllShipsSunk(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == '#')
            {
                return false;
            }
        }
    }
    return true;
}


void startGame(int option)
{
    char playerBoard[BOARD_SIZE][BOARD_SIZE];
    char opponentBoard[BOARD_SIZE][BOARD_SIZE];
    Ship playerShips[SHIP_TYPES][MAX_SHIPS_PER_TYPE];
    Ship opponentShips[SHIP_TYPES][MAX_SHIPS_PER_TYPE];

    clearBoard(playerBoard);
    clearBoard(opponentBoard);

  if(option == 1)
  {
    enterConfiguration(playerShips,playerBoard);
    enterConfiguration(opponentShips,opponentBoard);

  }else if(option == 2)
  {
    enterConfigurationFromFile("player.txt",playerShips,playerBoard);
    enterConfigurationFromFile("oponent.txt",opponentShips,opponentBoard);
  }

    int currentPlayer = 1;
    int opponentPlayer = 2;
    int currentShipsLeft = 31;
    int opponentShipsLeft = 31;

   while (true)
    {
        printf("Player %d, it's your turn.\n", 1);
        printf("Your board:\n");
        printBoard(playerBoard);
        printf("Opponent's board:\n");
        printBoard(opponentBoard);
        printf("Enter your guess:\n");

        int x, y;
        enterCoordinates(&x, &y);
      if (currentPlayer == 1)
        {
            updateOpponentBoard(x, y, opponentBoard, playerBoard);
            if (opponentBoard[y][x] == 'X')
            {
                printf("You hit an opponent's ship!\n");
                opponentShipsLeft--;
                if (opponentShipsLeft == 0)
                {
                    printf("Congratulations! Player 1 wins!\n");
                    break;
                }
            }
            else
            {
                printf("You missed.\n");
            }
        }
        else
        {
            updateOpponentBoard(x, y, playerBoard, opponentBoard);
            if (playerBoard[y][x] == 'X')
            {
                printf("You hit an opponent's ship!\n");
                currentShipsLeft--;
                if (currentShipsLeft == 0)
                {
                    printf("Congratulations! Player 2 wins!\n");
                    break;
                }
            }
            else
            {
                printf("You missed.\n");
            }
        }

        int temp = currentPlayer;
        currentPlayer = opponentPlayer;
        opponentPlayer = temp;
    }

}
void playGame(int option)//game against ai both ship configurations are taken from files player.txt and oponent.txt there is bug with hit detection for the ai
{
    char playerBoard[BOARD_SIZE][BOARD_SIZE];
    Ship playerShips[SHIP_TYPES][MAX_SHIPS_PER_TYPE];
    char AiBoard[BOARD_SIZE][BOARD_SIZE];
    Ship AiShips[SHIP_TYPES][MAX_SHIPS_PER_TYPE];
    int guesses = 0;
    int hits = 0;
    int lastHitShipIndex = -1;
    int currentShipsLeft = 31;
    int AiShipsLeft = 31;
    int x,y;

    clearBoard(playerBoard);
    clearBoard(AiBoard);

    enterConfigurationFromFile("player.txt",playerShips,playerBoard);
    enterConfigurationFromFile("oponent.txt",AiShips,AiBoard);


    while (hits < NUM_SHIPS)
    {
        Coordinate guess;

        printf("Player %d, it's your turn.\n", 1);
        printf("Your board:\n");
        printBoard(playerBoard);
        printf("Opponent's board:\n");
        printBoard(AiBoard);
        printf("Enter your guess:\n");
        enterCoordinates(&x, &y);

       updateOpponentBoard(x, y, playerBoard,AiBoard);

        if (AiBoard[y][x] == 'X')
            {
                printf("You hit an opponent's ship!\n");
                AiShipsLeft--;
                if (AiShipsLeft == 0)
                {
                    printf("Congratulations! Player 1 wins!\n");
                    return;
                }
            }
            else
            {
                printf("You missed.\n");
            }

        if (lastHitShipIndex != -1)
        {
            // Continue sinking the current ship
            Ship ship = ships[lastHitShipIndex];

            if (ship.isHorizontal)
            {
                guess.x = ship.x + (ship.length - 1) - (hits - ship.x);
                guess.y = ship.y;
            }
            else
            {
                guess.x = ship.x;
                guess.y = ship.y + (ship.length - 1) - (hits - ship.y);
            }
        }
        else
        {
            // Random guess if no ship is hit yet
            guess = getRandomCoordinate();
        }


        if (isHit(guess))
        {
            hits++;
            printf("Hit at (%c%d)!\n", 'A' + guess.x, guess.y + 1);

            for (int i = 0; i < NUM_SHIPS; i++)
            {
                if (isSunk(i))
                {
                    printf("Ship %d is sunk!\n", i + 1);
                    lastHitShipIndex = -1;  // Reset last hit ship index
                }
            }

            lastHitShipIndex = hits - 1;  // Update last hit ship index
        }
        else
        {
            printf("Miss at (%c%d).\n", 'A' + guess.x, guess.y + 1);
            lastHitShipIndex = -1;  // Reset last hit ship index
        }

        updateOpponentBoard(guess.x, guess.y, AiBoard, playerBoard);

        guesses++;
    }

    printf("Game Over! You sunk all the ships in %d guesses.\n", guesses);
}


void replay(char board[BOARD_SIZE][BOARD_SIZE])
{
    FILE *f;
    f = fopen("replay.txt", "a");
    time_t t;
    time(&t);
    fprintf(f, "\n---------------------------------\n");
    fprintf(f, "Game Date: %s", ctime(&t));
    fprintf(f, "---------------------------------");
    fprintf(f, "\nPlayer 1\n");
    fprintf(f, "~~~~~~~~~~~\n");
    fprintf(f, "   ");
    for (char c = 'A'; c < 'A' + BOARD_SIZE; c++)
    {
        fprintf(f, "%c ", c);
    }
    fprintf(f, "\n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        fprintf(f, "%2d ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            fprintf(f, "%c ", board[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}


int main()
{

    int command;
    printf("     Welcome to Battleships!     \n\n");
    printf("To start choose action (type number of the action):\n1.Start 1v1 game with custom ship configuration\n2.Start 1v1 game with custom ship configuration from file \n3.Start game with bot\n4.Encrypt replay\n5.Decrypt replay\n");
    scanf("%d", &command);

    switch(command)
    {
        case 1:startGame(1);break;
        case 2:startGame(2);break;
        case 3:playGame(1);break;
        case 4:encrypt();break;
        case 5:decrypt();break;
    }

    return 0;
}
