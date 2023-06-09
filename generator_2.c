#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#define BOARD_SIZE 10
#define SHIP_TYPES 4
#define MAX_SHIPS_PER_TYPE {4, 3, 2, 1}

typedef struct
{
    int x;
    int y;
    int length;
    bool isHorizontal;
} Ship;

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

bool isValidCoordinates(int x, int y)
{
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

bool isValidPlacement(Ship ship, char board[BOARD_SIZE][BOARD_SIZE])
{
    if (ship.isHorizontal)
    {
        if (!isValidCoordinates(ship.x, ship.y) ||
            !isValidCoordinates(ship.x + ship.length - 1, ship.y) ||
            ship.y < 0 || ship.y >= BOARD_SIZE)
        {
            return false;
        }
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
            !isValidCoordinates(ship.x, ship.y + ship.length - 1) ||
            ship.x < 0 || ship.x >= BOARD_SIZE)
        {
            return false;
        }
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

bool isOverlap(Ship ship, char board[BOARD_SIZE][BOARD_SIZE])
{
    if (ship.isHorizontal)
    {
        for (int i = 0; i < ship.length; i++)
        {
            if (board[ship.y][ship.x + i] != ' ')
            {
                return true;
            }
        }
    }
    else
    {
        for (int i = 0; i < ship.length; i++)
        {
            if (board[ship.y + i][ship.x] != ' ')
            {
                return true;
            }
        }
    }
    return false;
}

bool hasCommonSides(Ship ship, char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = -1; i <= ship.length; i++)
    {
        int x = ship.x + i;
        int y = ship.y - 1;
        if (isValidCoordinates(x, y) && board[y][x] != ' ')
        {
            return true;
        }

        y = ship.y + 1;
        if (isValidCoordinates(x, y) && board[y][x] != ' ')
        {
            return true;
        }
    }

    int x = ship.x - 1;
    int y = ship.y;
    if (isValidCoordinates(x, y) && board[y][x] != ' ')
    {
        return true;
    }

    x = ship.x + ship.length;
    if (isValidCoordinates(x, y) && board[y][x] != ' ')
    {
        return true;
    }

    return false;
}

bool hasCommonCorners(Ship ship, char board[BOARD_SIZE][BOARD_SIZE])
{
    int x, y;

    x = ship.x - 1;
    y = ship.y - 1;
    if (isValidCoordinates(x, y) && board[y][x] != ' ')
    {
        return true;
    }

    x = ship.x - 1;
    y = ship.y + 1;
    if (isValidCoordinates(x, y) && board[y][x] != ' ')
    {
        return true;
    }

    x = ship.x + ship.length;
    y = ship.y - 1;
    if (isValidCoordinates(x, y) && board[y][x] != ' ')
    {
        return true;
    }

    x = ship.x + ship.length;
    y = ship.y + 1;
    if (isValidCoordinates(x, y) && board[y][x] != ' ')
    {
        return true;
    }

    return false;
}

void generateStartingPositions()
{
    char board[BOARD_SIZE][BOARD_SIZE];
    clearBoard(board);

    const int maxShipsPerType[SHIP_TYPES] = MAX_SHIPS_PER_TYPE;
    const int maxShipLength[SHIP_TYPES] = {2,3,4,6};

    FILE* file = fopen("bot_starting.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    for (int i = SHIP_TYPES - 1; i >= 0; i--)
    {
        int numShips = maxShipsPerType[i];
        int shipLength = maxShipLength[i];

        for (int j = 0; j < numShips; j++)
        {
            Ship ship;
            ship.length = shipLength;

            // Randomly determine orientation
            ship.isHorizontal = rand() % 2 == 0;

            // Randomly generate starting coordinates
            if (ship.isHorizontal)
            {
                ship.x = rand() % (BOARD_SIZE - ship.length + 1);
                ship.y = rand() % BOARD_SIZE;
            }
            else
            {
                ship.x = rand() % BOARD_SIZE;
                ship.y = rand() % (BOARD_SIZE - ship.length + 1);
            }

            if (isValidPlacement(ship, board) && !isOverlap(ship, board) &&
                !hasCommonSides(ship, board) && !hasCommonCorners(ship, board))
            {
                printf("Ship number %d with size %d has passed the requirements!\n", j + 1, ship.length);

                if (ship.isHorizontal)
                {
                    for (int k = 0; k < ship.length; k++)
                    {
                        board[ship.y][ship.x + k] = 'X';
                    }
                }
                else
                {
                    for (int k = 0; k < ship.length; k++)
                    {
                        board[ship.y + k][ship.x] = 'X';
                    }
                }

                char orientation = ship.isHorizontal ? 'h' : 'v';
                fprintf(file, "%c%d %c\n", ship.x + 'A', ship.y + 1, orientation);
            }
            else
            {
                j--; // Retry generating the same ship type
            }
        }
    }

    fclose(file);
    printf("Starting positions generated and saved in bot_starting.txt.\n");
}

void readAndPrintStartingPositions()
{
    FILE* inputFile = fopen("bot_starting.txt", "r");
    if (inputFile == NULL)
    {
        printf("Error opening input file.\n");
        return;
    }

    FILE* outputFile = fopen("botka.txt", "w");
    if (outputFile == NULL)
    {
        printf("Error opening output file.\n");
        fclose(inputFile);
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        fprintf(outputFile, "%s", line);
    }

    fclose(inputFile);
    fclose(outputFile);
    printf("Ship positions read from bot_starting.txt and printed to botka.txt in reverse order.\n");
}
void final_check()
{
    FILE* inputFile = fopen("bot_starting.txt", "r");
    if (inputFile == NULL)
    {
        printf("Error opening input file.\n");
        return;
    }

    char board[BOARD_SIZE][BOARD_SIZE];
    clearBoard(board);

    char line[100];
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        int x, y;
        char orientation;
        sscanf(line, "%c%d %c", &x, &y, &orientation);
        x -= 'A';
        y--;

        Ship ship;
        ship.x = x;
        ship.y = y;
        ship.isHorizontal = (orientation == 'h');
        ship.length = (orientation - 'A');

        if (!isValidPlacement(ship, board) || isOverlap(ship, board))
        {
            printf("Ship position is invalid or overlaps with another ship.\n");
            printf("Moving ship to a new position.\n");

            bool foundValidPosition = false;
            while (!foundValidPosition)
            {
                if (ship.isHorizontal)
                {
                    ship.x = rand() % (BOARD_SIZE - ship.length + 1);
                    ship.y = rand() % BOARD_SIZE;
                }
                else
                {
                    ship.x = rand() % BOARD_SIZE;
                    ship.y = rand() % (BOARD_SIZE - ship.length + 1);
                }

                if (isValidPlacement(ship, board) && !isOverlap(ship, board))
                {
                    foundValidPosition = true;
                }
            }

            printf("Ship position moved to (%c%d %c).\n", ship.x + 'A', ship.y + 1, orientation);
        }

        if (ship.isHorizontal)
        {
            for (int k = 0; k < ship.length; k++)
            {
                board[ship.y][ship.x + k] = 'X';
            }
        }
        else
        {
            for (int k = 0; k < ship.length; k++)
            {
                board[ship.y + k][ship.x] = 'X';
            }
        }
    }

    fclose(inputFile);
    printf("All ships passed the final check.\n");
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

  printf("\npass:%s,time:%d,key:%s,avr:%d", password, time(NULL) % 254 + 1, key,
         get_avarage_value_of_string(password));

  return key;
}

void encrypt() {
  FILE *fdata = fopen("replay.txt", "rb+");
  int fileSize = get_file_size(fdata);
  char *file_data = (char *)malloc(fileSize + 1);
  char *output = (char *)malloc(fileSize + 2);
  char *key[100];
  rewind(fdata);

  output[0] = key[0];

  printf("Enter your pasword:");
  scanf("%s", key);

  if (fdata == NULL) {
    printf("File can't open");
    return;
  }

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
  free(file_data);
}

char *decrypt() {
  FILE *fdata = fopen("replay.txt", "rb+");
  int fileSize = get_file_size(fdata);
  char *file_data = (char *)malloc(fileSize + 1);
  char psw[100], key[100];
  rewind(fdata);

  printf("Enter your pasword:");
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
  free(file_data);
}

void replay(char board[BOARD_SIZE][BOARD_SIZE])
{
    FILE *f;
    f = fopen("r", "a");
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
    char playerBoard[BOARD_SIZE][BOARD_SIZE];
    char opponentBoard[BOARD_SIZE][BOARD_SIZE];
    Ship playerShips[SHIP_TYPES][MAX_SHIPS_PER_TYPE];
    Ship playerShips[SHIP_TYPES][MAX_SHIPS_PER_TYPE];
    Ship opponentShips[SHIP_TYPES][MAX_SHIPS_PER_TYPE];

    clearBoard(playerBoard);
    clearBoard(opponentBoard);

    char load_from_file;
    printf("Player 1, type f to load a starting position from a file or type m to do it manually: ");
    scanf("%c", &load_from_file);
    getchar();
    if(load_from_file == 'f')
    {
        printf("Player 1, enter the name of your ship configuration file: ");
        char player1ConfigFile[50];
        scanf("%s", player1ConfigFile);
        getchar();
        enterConfigurationFromFile(player1ConfigFile, playerShips, playerBoard);



    }
    else
    {
        printf("Player 1, enter your ship configurations:\n");
        enterConfiguration(playerShips, playerBoard);
    }
    printf("Player 2, type f to load a starting position from a file or type m to do it manually: ");
    char load_from_file_2;
    scanf("%c", &load_from_file_2);
    getchar();
    if(load_from_file_2 == 'f')
    {
        printf("Player 2, enter the name of your ship configuration file: ");
        char player2ConfigFile[50];
        scanf("%s", player2ConfigFile);
        getchar();
        enterConfigurationFromFile(player2ConfigFile, opponentShips, opponentBoard);

    }
    else
    {
        printf("Player 2, enter your ship configurations:\n");
        enterConfiguration(opponentShips, opponentBoard);
    }

    int currentPlayer = 1;
    int opponentPlayer = 2;
    int currentShipsLeft = 31;
    int opponentShipsLeft = 31;


    while (true)
    {
        printf("Player %d, it's your turn.\n", currentPlayer);
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

    return 0;
}


