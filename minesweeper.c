// Assignment 1 20T1 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by Aaron Wang (z5308498)
// on 23/03/2020
//
// Version 1.0.0 (2020-03-08): Assignment released.
// Version 1.0.1 (2020-03-08): Fix punctuation in comment.
// Version 1.0.2 (2020-03-08): Fix second line of header comment to say minesweeper.c

#include <stdio.h>
#include <stdlib.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE 8

// The possible command codes.
#define DETECT_ROW              1
#define DETECT_COL              2
#define DETECT_SQUARE           3
#define REVEAL_SQUARE           4
#define GAMEPLAY_MODE           5
#define DEBUG_MODE              6
#define REVEAL_RADIAL           7

// Maximum number of hints allowed to be used in a game.
#define MAX_HINTS 3

void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void initialise_gameMinefield(int gameMinefield[SIZE + 3][SIZE + 3]);
void place_mines(int minefield[SIZE][SIZE]);
void count_mines(int minefield[SIZE][SIZE], int command, int index);
int detect_square(int minefield[SIZE][SIZE], int row, int col, int size);
void reveal_square(int minefield[SIZE][SIZE], int *gameEnd, int *gameLost);
void check_win(int minefield[SIZE][SIZE], int *gameEnd);
void update_game_minefield(int minefield[SIZE][SIZE],
                           int gameMinefield[SIZE + 3][SIZE + 3]);
void print_game_minefield(int gameMinefield[SIZE + 3][SIZE + 3],
                          int gameLost);

// Place your function prototyes here.

int main(void) {

    int minefield[SIZE][SIZE];
    initialise_field(minefield);
    int gameMinefield[SIZE + 3][SIZE + 3];
    // Initialise the game version of the minefield.
    initialise_gameMinefield(gameMinefield);
    printf("Welcome to minesweeper!\n");

    // Scan number of mines and their coordinates and then places mines.
    place_mines(minefield);

    printf("Game Started\n");
    print_debug_minefield(minefield);

    int command;
    // gameEnd has two states: 0 for not ended and 1 for ended.
    int gameEnd = 0;
    // gameLost has two states: 0 for not lost and 1 for lost.
    int gameLost = 0;
    int hintCount = 0;
    // gameMode has two states: 0 for debug mode and 1 for gameplay mode.
    int gameMode = 0;

    // While not EOF and game has not ended
    while (scanf("%d", &command) == 1 && gameEnd == 0) {
        
        if (command == DETECT_ROW) {

            // The row/column the user wants to check
            int index;
            scanf("%d", &index);

            if (hintCount < MAX_HINTS) {

                // Count the number of mines in a row or column.
                count_mines(minefield, DETECT_ROW, index);
                // Increment the hint counter
                hintCount++;
                
            } else {
                printf("Help already used\n");
            }
            
        } else if (command == DETECT_COL) {

            // The row/column the user wants to check
            int index;
            scanf("%d", &index);

            if (hintCount < MAX_HINTS) {
            
                // Count the number of mines in a row or column.
                count_mines(minefield, DETECT_COL, index);
                hintCount++;
                
            } else {
                printf("Help already used\n");
            }
            
        } else if (command == DETECT_SQUARE) {
            
            int row, col, size;

            scanf("%d", &row);
            scanf("%d", &col);
            scanf("%d", &size);

            if (hintCount < MAX_HINTS) {

                if (size % 2 != 0 && size >= 1 && size <= SIZE) {
                    
                    // Count the number of mines in an n*n section
                    // of the grid.
                    int minesCounted = (detect_square(minefield, row,
                                        col, size));
                    
                    printf("There are %d mine(s) in the square "
                    "centered at row %d, column %d of size %d\n",
                    minesCounted, row, col, size);

                }

                hintCount++;

            } else {
                printf("Help already used\n");
            }
            
        } else if (command == REVEAL_SQUARE) {
        
            // Reveal a 3*3 section of the grid.
            reveal_square(minefield, &gameEnd, &gameLost);

            if (gameLost == 1) {

                printf("Game over\n");

            } else if (gameLost == 0) {
            
                // Check if the game has been won.
                check_win(minefield, &gameEnd);

            }
            
        } else if (command == GAMEPLAY_MODE) {
        
            printf("Gameplay mode activated\n");
            // Set gameMode to gameplay mode.
            gameMode = 1;
            
        } else if (command == DEBUG_MODE) {
        
            printf("Debug mode activated\n");
            // Set gameMode to gameplay mode.
            gameMode = 0;

        }

        if (gameMode == 0) {
        
            // Print the debug version of the minefield after each command.
            print_debug_minefield(minefield);
            
        } else if (gameMode == 1) {

            // Update the game version of the minefield after each command.
            update_game_minefield(minefield, gameMinefield);
            
            // Print the game version of the minefield after each command.
            print_game_minefield(gameMinefield, gameLost);
            
        }
        
    }
    
    return 0;
}

// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            minefield[i][j] = HIDDEN_SAFE;
            j++;
        }
        i++;
    }
}

// Print out the actual values of the minefield.
void print_debug_minefield(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", minefield[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

// Initialise the game version of the minefield.
void initialise_gameMinefield(int gameMinefield[SIZE + 3][SIZE + 3]) {

    // This will setup the game minefield with all elements initialised to
    // HIDDEN_SAFE.
    // This will also setup the non-minefield elements of the minesweeper
    // game by initialising an array with integers that will correspond to
    // different things printed by the print_gameMinefield function.
    // The purpose of this is to accomodate changes to the SIZE constant,
    // as hardcoding the non-minefield elements would not work for a
    // different sized minefield.
    // This will allow the display to remain functional for SIZEs up to 99. 
    
    // A key is included below (and is included in the print function too):
    // Values of 0 to 7 will be printed as "00, 01, 02... 07"
    // Values of 10 to 99 will be printed as normal.
    // A value of 100 will be printed as "  " (two spaces)
    // A value of 101 will be printed as " " (one space)
    // A value of 102 will be printed as "---"
    // A value of 103 will be printed as " -"
    // A value of 104 will be printed as "|"

    int row = 0;
    while (row < SIZE + 3) {
    
        int col = 0;
        while (col < SIZE + 3) {

            if (row == 0) {

                if (col == 0 || col == 1) {
                    // A value of 100 will be printed as "  " (two spaces)
                    gameMinefield[row][col] = 100;
                } else if (col == SIZE + 2) {
                    // A value of 101 will be printed as " " (one space)
                    gameMinefield[row][col] = 101;
                } else {
                    // Values of 0 to 7 will be printed as "00, 01, 02... 07"
                    // and values of 10 to 99 will be printed as normal.
                    gameMinefield[row][col] = col - 2;
                }
                    
            } else if (row == 1 || row == SIZE + 2) {

                if (col == 0) {
                    // A value of 100 will be printed as "  " (two spaces)
                    gameMinefield[row][col] = 100;
                } else if (col == 1) {
                    // A value of 103 will be printed as " -"
                    gameMinefield[row][col] = 103;
                } else if (col < SIZE + 2) {
                    // A value of 102 will be printed as "---"
                    gameMinefield[row][col] = 102;
                } else {
                    // A value of 101 will be printed as " " (one space)
                    gameMinefield[row][col] = 101;
                }
                 
            } else {

                if (col == 0) {
                    // Values of 0 to 7 will be printed as "00, 01, 02... 07"
                    gameMinefield[row][col] = row - 2; 
                } else if (col == 1 || col == SIZE + 2) {
                    // A value of 104 will be printed as "|"
                    gameMinefield[row][col] = 104;
                } else {
                    // All minefield elements are initialised to HIDDEN_SAFE
                    gameMinefield[row][col] = HIDDEN_SAFE;
                } 

            }
            col++;
            
        }
        row++;
        
    }
    return;
}

// Scan number of mines and their coordinates and then places mines.
void place_mines(int minefield[SIZE][SIZE]) {

    int numMines;

    printf("How many mines? ");
    
    scanf("%d", &numMines);
    
    printf("Enter pairs:\n");

    int i = 0;
    while (i < numMines) {

        int row, col;
        scanf("%d %d", &row, &col);
        if (row >= 0 && row < SIZE && col >= 0 && col < SIZE) {
            minefield[row][col] = HIDDEN_MINE;
        }
        i++;
        
    }

    return;
}

// Count the number of mines in a row or column.
void count_mines(int minefield[SIZE][SIZE], int command, int index) {

    int minesCounted = 0;
    // Counter to iterate through the detected row/column
    int i = 0;
    while (i < SIZE) {

        if (command == DETECT_ROW) {
         
            if (minefield[index][i] == HIDDEN_MINE) {
                minesCounted++;
            }
            i++;
            
        } else if (command == DETECT_COL) {
        
            if (minefield[i][index] == HIDDEN_MINE) {
                minesCounted++;
            }
            i++;
            
        }
    }

    if (command == DETECT_ROW) { 
        printf("There are %d mine(s) "
        "in row %d\n", minesCounted, index);						
    } else if (command == DETECT_COL) {
        printf("There are %d mine(s) "
        "in column %d\n", minesCounted, index);
    }
    
    return;
}

// Count the number of mines in an n*n section.
int detect_square(int minefield[SIZE][SIZE], int row, int col, int size) {

    int minesCounted = 0;

    // The index counter for detecting the squares around the centre
    int i = 0;

    // While the detection diameter is <= the size of the square
    while ((i * 2 + 1) <= size) {

        if (i == 0) {
            if (minefield[row][col] == HIDDEN_MINE) {
                // centre
                minesCounted++;
            }
        } else {

            if (row + 1 < SIZE && col + 1 < SIZE) {
                if (minefield[row + i][col + i] == HIDDEN_MINE) {
                    // right down
                    minesCounted++;
                }
            }
            if (row - 1 > -1 && col - 1 > -1) {
                if (minefield[row - i][col - i] == HIDDEN_MINE) {
                    // left up
                    minesCounted++;
                }
            }
            if (row + 1 < SIZE && col - 1 > -1) {
                if (minefield[row + i][col - i] == HIDDEN_MINE) {
                    // left down
                    minesCounted++;
                }
            }
            if (row - 1 > -1 && col + 1 < SIZE) {
                if (minefield[row - i][col + i] == HIDDEN_MINE) {
                    // right up
                    minesCounted++;
                }
            }
            if (col - 1 > -1) {
                if (minefield[row][col - i] == HIDDEN_MINE) {
                    // left
                    minesCounted++;
                }
            }
            if (col + 1 < SIZE) {
                if (minefield[row][col + i] == HIDDEN_MINE) {
                    // right
                    minesCounted++;
                }
            }
            if (row - 1 > -1) {
                if (minefield[row - i][col] == HIDDEN_MINE) {
                    // up
                    minesCounted++;
                }
            }
            if (row + 1 < SIZE) {
                if (minefield[row + i][col] == HIDDEN_MINE) {
                    // down
                    minesCounted++;
                }
            }
                        
        }
        i++;
        
    }
    
    return minesCounted;
}

// Reveal a 3*3 section of the grid.
void reveal_square(int minefield[SIZE][SIZE], int *gameEnd, int *gameLost) {

    int row, col;
    int size = 3;    

    scanf("%d ", &row);
    scanf("%d", &col);

    if (minefield[row][col] == HIDDEN_MINE) {
    
        *gameLost = 1;
        *gameEnd = 1;
        
    } else {
    
        // If the number of mines counted in an 3*3 square centered at
        // [row][col] > 0, don't reveal the square
        if (detect_square(minefield, row, col, size) > 0) {
            minefield[row][col] = VISIBLE_SAFE;
        } else if (detect_square(minefield, row, col, size) == 0) {

            minefield[row][col] = VISIBLE_SAFE;
            // Checking if the square is inside the grid 
            if (row + 1 < SIZE && col + 1 < SIZE) {
                // right down
                minefield[row + 1][col + 1] = VISIBLE_SAFE;
            }
            if (row - 1 > -1 && col - 1 > -1) {
                // left up
                minefield[row - 1][col - 1] = VISIBLE_SAFE;
            }
            if (row + 1 < SIZE && col - 1 > -1) {
                // left down
                minefield[row + 1][col - 1] = VISIBLE_SAFE;
            }
            if (row - 1 > -1 && col + 1 < SIZE) {
                // right up
                minefield[row - 1][col + 1] = VISIBLE_SAFE;
            }
            if (col - 1 > -1) {
                // left
                minefield[row][col - 1] = VISIBLE_SAFE;
            }
            if (col + 1 < SIZE) {
                // right
                minefield[row][col + 1] = VISIBLE_SAFE;
            }
            if (row - 1 > -1) {
                // up
                minefield[row - 1][col] = VISIBLE_SAFE;
            }
            if (row + 1 < SIZE) {
                // down
                minefield[row + 1][col] = VISIBLE_SAFE;
            }
        }
        
    }  
    
    return;
}

// Check if the game has been won.
void check_win(int minefield[SIZE][SIZE], int *gameEnd) {

    int won = 1;

    int row = 0;
    while (row < SIZE) {
    
        int col = 0;
        while (col < SIZE) {

            if (minefield[row][col] == HIDDEN_SAFE) {
                won = 0;
                col = SIZE;
                row = SIZE;    
            }
            col++;
            
        }
        row++;
        
    }

    if (won == 1) {
        printf("Game Won!\n");
        *gameEnd = 1;
    }

    return;
}

void update_game_minefield(int minefield[SIZE][SIZE],
                           int gameMinefield[SIZE + 3][SIZE + 3]) {

    // Key for printing minefield elements:
    // Values of 110 (HIDDEN_SAFE) will be printed as "## "
    // Values of 111 (HIDDEN_MINE) will be printed as "## ", unless the game
    // is lost, in which case it will be printed as "() "
    // Values of 112 (VISIBLE_SAFE but has no adajacent mines)
    // will be printed as "   " (three spaces)

    int adjacentMines;
    int size = 3;    

    int row = 0;
    while (row < SIZE) {
    
        int col = 0;
        while (col < SIZE) {

            if (minefield[row][col] == HIDDEN_SAFE) {

                // gameMinefield[row + 2][col + 2] is used as the other
                // array spaces are occupied by non-minefield elements
                // Values of 110 will be printed as "##"
                gameMinefield[row + 2][col + 2] = 110;
                    
            } else if (minefield[row][col] == VISIBLE_SAFE) {

                adjacentMines = detect_square(minefield, row, col, size);
                // gameMinefield[row][col] becomes the number of adjacent
                // mines to the current grid coordinate.
                if (adjacentMines > 0) {
                    gameMinefield[row + 2][col + 2] = adjacentMines;
                } else {
                    // Values of 112 will be printed as "  "
                    gameMinefield[row + 2][col + 2] = 112;
                }
                
            } else if (minefield[row][col] == HIDDEN_MINE) {

                // Values of 111 will be printed as "##", unless the game
                // is lost, in which case it will be printed as "()"
                gameMinefield[row + 2][col + 2] = 111;
                
            }
            col++;
            
        }
        row++;
        
    }
    return;
}

// Print the gameplay version of the minefield.
void print_game_minefield(int gameMinefield[SIZE + 3][SIZE + 3],
                          int gameLost) {

    if (gameLost == 0) {
        printf("..\n");
        printf("\\/\n");
    } else if (gameLost == 1) {
        printf("xx\n");
        printf("/\\\n");
    } 

    // Key for printing non-minefield elements:
    // Values of 0 to 9 will be printed as "00, 01, 02... 09"
    // Values of 10 to 99 will be printed as normal.
    // A value of 100 will be printed as "  " (two spaces)
    // A value of 101 will be printed as " " (one space)
    // A value of 102 will be printed as "---"
    // A value of 103 will be printed as " -"
    // A value of 104 will be printed as "|"

    // Key for printing minefield elements:
    // Values of 110 (HIDDEN_SAFE) will be printed as "## "
    // Values of 111 (HIDDEN_MINE) will be printed as "## ", unless the game
    // is lost, in which case it will be printed as "()"
    // Values of 112 (VISIBLE_SAFE but has no adajacent mines)
    // will be printed as "   " (three spaces)

    int row = 0;
    while (row < SIZE + 3) {
    
        int col = 0;
        while (col < SIZE + 3) {

            if (gameMinefield[row][col] >= 0 &&
                gameMinefield[row][col] <= 9) {
                // Values of 0 to 9 will be printed as "00, 01, 02... 09"
                if (col == SIZE + 1) {
                    // This is done to format the minefield properly.
                    printf("0%d", gameMinefield[row][col]);
                } else {
                    printf("0%d ", gameMinefield[row][col]);
                }

            } else if (gameMinefield[row][col] >= 10 &&
                       gameMinefield[row][col] <= 99) {
                // Values of 10 to 99 will be printed as normal.
                if (col == SIZE + 1) {
                    // This is done to format the minefield properly.
                    printf("%d", gameMinefield[row][col]);
                } else {
                    printf("%d ", gameMinefield[row][col]);
                }
                
            } else if (gameMinefield[row][col] == 110) {
                // Values of 110 (HIDDEN_SAFE) will be printed as "## "
                if (col == SIZE + 1) {
                    // This is done to format the minefield properly.
                    printf("##");
                } else {
                    printf("## ");
                }    
            } else if (gameMinefield[row][col] == 111) {
                // Values of 111 (HIDDEN_MINE) will be printed as "## ",
                // unless the game is lost, in which case it will be 
                // printed as "()"
                if (gameLost == 0) {
                
                    if (col == SIZE + 1) {
                        // This is done to format the minefield properly.
                        printf("##");
                    } else {
                        printf("## ");
                    }
                    
                } else if (gameLost == 1) {
                    if (col == SIZE + 1) {
                        // This is done to format the minefield properly.
                        printf("()");
                    } else {
                        printf("() ");
                    }
                    
                }
            } else if (gameMinefield[row][col] == 100) {
                // A value of 100 will be printed as "  " (two spaces)
                printf("  ");
            } else if (gameMinefield[row][col] == 112) {
                // Values of 112 (VISIBLE_SAFE but has no adajacent mines)
                // will be printed as "   " (three spaces)
                if (col == SIZE + 1) {
                    // This is done to format the minefield properly.
                    printf("  ");
                } else {
                    printf("   ");
                }
            } else if (gameMinefield[row][col] == 101) {
                // A value of 101 will be printed as " " (one space)
                printf(" ");
            } else if (gameMinefield[row][col] == 102) {
                // A value of 102 will be printed as "---"
                printf("---");
            } else if (gameMinefield[row][col] == 103) {
                // A value of 103 will be printed as " -"
                printf(" -");
            } else if (gameMinefield[row][col] == 104) {
                // A value of 104 will be printed as "|"
                printf("|");
            }
            col++;
            
        }
        printf("\n");
        row++;
        
    }
    return;
}
