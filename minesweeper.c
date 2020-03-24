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

void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void place_mines(int minefield[SIZE][SIZE]);
void count_mines(int minefield[SIZE][SIZE], int command);
int detect_square(int minefield[SIZE][SIZE], int row, int col, int size);
void reveal_square(int minefield[SIZE][SIZE], int *gameEnd);
void check_win(int minefield[SIZE][SIZE], int *gameEnd);

// Place your function prototyes here.

int main(void) {

    int minefield[SIZE][SIZE];
    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");

    // Scan number of mines and their coordinates and then places mines.
    place_mines(minefield);

    printf("Game Started\n");
    print_debug_minefield(minefield);

    int command;
    // gameEnd has two states: 0 for not ended and 1 for ended
    int gameEnd = 0;

    // While not EOF and game has not ended
    while (scanf("%d ", &command) == 1 && gameEnd == 0) {
        
        if (command == DETECT_ROW) {
            // Count the number of mines in a row or column.
            count_mines(minefield, DETECT_ROW);
        } else if (command == DETECT_COL) {
            // Count the number of mines in a row or column.
            count_mines(minefield, DETECT_COL);
        } else if (command == DETECT_SQUARE) {

            int row, col, size;

            scanf("%d ", &row);
            scanf("%d ", &col);
            scanf("%d", &size);

            if (size % 2 != 0 && size >= 1 && size <= SIZE) {
                
                // Count the number of mines in an n*n section of the grid.
                int minesCounted = (detect_square(minefield, row, col, size));
                
                printf("There are %d mine(s) in the square centered at row "
                "%d, column %d of size %d\n", minesCounted, row, col, size);

            }
            
        } else if (command == REVEAL_SQUARE) {
            // Reveal a 3*3 section of the grid.
            reveal_square(minefield, &gameEnd);
            // Check if the game has been won.
            check_win(minefield, &gameEnd);
        }

        // Print minefield after each command
        print_debug_minefield(minefield); 
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
void count_mines(int minefield[SIZE][SIZE], int command) {

    // The row/column the user wants to check
    int index;
    scanf("%d", &index);

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
void reveal_square(int minefield[SIZE][SIZE], int *gameEnd) {

    int row, col;
    int size = 3;    

    scanf("%d ", &row);
    scanf("%d", &col);

    if (minefield[row][col] == HIDDEN_MINE) {
        printf("Game over\n");
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
