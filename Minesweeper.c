/** \file
* \brief This file contains the main program for a minesweeper game */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "setup.h"
#include "gamestatistics.h"

// function prototypes
void printField(Cell** field);
void guess(Cell** field);
void openCell(int x, int y, Cell** field);
void revealAllMines(Cell** field);
int lost(Cell** field);
int playAgain(Cell** field);
int allOpened(Cell** field);
int allFlagged(Cell** field);


int main(int argc, char *argv[]){
  grid_size = 10;
  no_of_mines = 20;
  // use command line arguments for grid size and mine count, if given
  if(argc > 1){
    char *a = argv[1];
    int grid = atoi(a);
    if(grid < 5 || grid > 24){
      printf("Please supply a grid size between 5 and 24 as first command line argument.\n");
      exit(1);
    }
    grid_size = grid;
  }
  if(argc > 2){
    char *a = argv[1];
    int grid = atoi(a);
    char *b = argv[2];
    int mines = atoi(b);
    if(mines < 2 || mines > ((grid * grid)/2)){
      printf("Please supply a mine count > 2 as second command line argument, but not higher than half the number of cells.\n");
      exit(1);
    }
    no_of_mines = mines;
  }
  do {
    loadStatistics();
    srand(time(NULL));
    field = generateField();
    printField(field);
    while (allOpened(field) == 0 && allFlagged(field) == 0 && lost(field) == 0){
      guess(field);
      printField(field);
    }
  } while (playAgain(field)==1);
  return 0;
}


/**
 Prints the playing field.

 Prints the playing field (a nested array of cells) with X (letters) and Y (nnumbers) coordinates.
 Depending on the flags set on a cell, the following output is printed:
 isFlagged == 1 : '?'
 isOpened == 0 : '-'
 isOpened == 1 && mine == 0 : '<number of adjacent mines'
 isOpened == 1 && mine == 1 : 'M'

 \param field a pointer to a nested array of cells
 */
void printField(Cell** field){
  // print x coordinates
  printf(" \t");
  for (int i = 0 ; i < grid_size; i++ ){
    printf("%c", i+65);
    printf(" \t");
  }
  printf(" \n");
  // print field
  // print y coordinates
  for (int i = 0 ; i < grid_size; i++ ){
    printf("%d", i+1);
    printf(" \t");
    // print cells
    for (int j = 0 ; j < grid_size; j++ ){
      if(field[i][j].isFlagged == 1 && field[i][j].isOpened == 0) {
        printf("?\t");
      }
      else if(field[i][j].isOpened == 1 ){
        if(field[i][j].mine == 1){
          printf("M\t");
        }
        else if (field[i][j].adjacentMines == 0){
          printf(" \t");
        } else {
        printf("%d\t", field[i][j].adjacentMines);}
        }
       else {
        printf("-\t");
      }
    }
  printf(" \n");
  }
}



/**
 Lets the player choose a cell to reveal.

 Prompts player to choose a cell to reveal.
 Validates player input, reveals all mines if a mine was hit, reveals chosen cell otherwise.
 Alternatively flags cell as potentially containing a mine if '?' option was specified in front of the coordinates.

 \param field a pointer to a nested array of cells
 */
void guess(Cell** field){
  printf("Enter the cell you want to target in the format A2 and hit Enter.\n");
  char str[5];
  fgets(str, 5, stdin);
  fflush(stdin);
  int x;
  int y;

  // converts player input into array indices
  y = (int)str[0]-65;
  x = atoi(&str[1])-1;

  if(str[0]== '?'){
  y = (int)str[1]-65;
  x = atoi(&str[2])-1;
  }
  // prompts player again for input if input was invalid
  if (x < 0 || y < 0 || x > grid_size-1 || y > grid_size-1) {
    printf("Please target an existing cell.\n");
    guess(field);
  }
  // flags mine if '?' was specified
  else if (str[0]== '?' && field[x][y].isOpened == 0){
    field[x][y].isFlagged = 1;
  }
  // prompts player again for input if input cell was already opened
  else if (field[x][y].isOpened == 1){
    printf("Please target an unopened cell.\n");
    guess(field);
  }
  // reveals all mines if a mine was hit
  else if (field[x][y].mine == 1) {
    revealAllMines(field);
    printf("Oh No. You hit a mine. You lost.\n");
    statistics[3]++; // cells opened
    statistics[2]++; // games lost
  } else {
    openCell(x, y , field);
    statistics[3]++; // cells opened
  }
};

/**
 Opens a cell

 Sets isOpen to 1 and isFlagged to 0.
 In case of no neighbouring mines, calls itself recursively for all neighbour cells with 0 neighbouring mine.
 \param  i integer x coordinate of cell to reveal
 \param  j integer y coordinate of cell to reveal
 \param field a pointer to a nested array of cells
 */
void openCell(int i, int j, Cell** field){
  if (field[i][j].isOpened == 1) {
    return;
  }
  field[i][j].isOpened = 1;
  field[i][j].isFlagged = 0;
  // recursively call openCell for all neighbour cells with 0 neighbouring mines
  if (field[i][j].adjacentMines == 0) {
    if (j-1 >= 0 && field[i][j-1].adjacentMines == 0) {
      openCell(i,j-1, field);
      }
    if (i-1 >= 0 && field[i-1][j].adjacentMines == 0) {
      openCell(i-1,j, field);
    }
    if (i+1 < grid_size && field[i+1][j].adjacentMines == 0) {
      openCell(i+1,j, field);
    }
    if (j+1 < grid_size && field[i][j+1].adjacentMines == 0) {
      openCell(i,j+1, field);
    }
  }
}

/**
 Checks if all none mine cells are opened
If all non-mine cells were opened , prints a winning massage and updates statistics.

 \param field a pointer to a nested array of cells

 \return integer indicating if all cells are opened (1) or not (0)

 */
int allOpened(Cell** field){
  for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].mine == 0 && field[i][j].isOpened == 0) {
        return 0;
      }
    }
  }
  printf("All cells are opened.\n");
  printf("You won!\n");
  statistics[0]++; // games played
  return 1;
}

/**
 Checks if all mine cells are flagged.

 Counts correctly flagged cells.
 Prints amount of mines in the game minus the amount of marked mines. Also show if this sum is negative, meaning a cell was marked as mine which does not have a mine.
 If all mines were flagged correctly, print a winning massage and updates statistics.

 \param field a pointer to a nested array of cells

 \return result indicating if all mines are flagged (1) or not (0)

 */
int allFlagged(Cell** field){
  int count = 0;
  int flagCount = 0;
  for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].mine == 1 && field[i][j].isFlagged == 1) {
        count++;
      }
      if (field[i][j].isFlagged == 1) {
        flagCount++;
      }
    }
  }

  int result = count == no_of_mines ? 1 : 0;
  // Show amount of mines in the game minus the amount of marked mines. Also show if this sum is negative, meaning a cell was marked as mine which does not have a mine.
  printf("There are %d mines.\n", no_of_mines - flagCount);
  if(count == no_of_mines) {
    printf("All mines flagged!\n");
    printf("You won!\n");
    statistics[1]++; // games won
    }
  return result;
}

/**
 Checks if the game is lost (i.e. a mine has been revealed)

 Checks for every cell if both mine and isOpened are 1.

 \param field a pointer to a nested array of cells

 \return result indicating if the game is lost (1) or not (0)

 */
int lost(Cell** field){
  for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].mine == 1 && field[i][j].isOpened == 1) {
        return 1;
      }
    }
  }
  return 0;
}

/**
 Reveals all mines.

 Set isOpened to 1 for every cell containing a mine.

 \param field a pointer to a nested array of cells

 */
void revealAllMines(Cell** field) {
   for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].mine == 1) {
        field[i][j].isOpened = 1;
      }
    }
  }
}

/**
 Restarts or ends game.

Prompts player if they want to play again.
Yes: calls saveStatistics(), frees the memory for the current playing field.
No: calls saveStatistics(), frees the memory for the current playing field, exits the program.

 \param field a pointer to a nested array of cells

 */

int playAgain(Cell** field){
char input[2];
 printf("Would you like to play again? y/n \n");
 scanf("%c", &input[0]);
 fflush(stdin);
  if(input[0] == 'y' ||input[0] == 'Y'){
    saveStatistics();
    printf("Ok, let's play again. \n");
    for (int i = 0; i < grid_size; i++){
      free(field[i]);
    }
    free(field);
    return 1;
  }
  else if (input[0] == 'n' || input[0] == 'N' ) {
    saveStatistics();
    printf("Ok, goodbye. \n");
    for (int i = 0; i < grid_size; i++){
      free(field[i]);
    }
    free(field);
    exit(0);
    return 1;
  }
  else {
    printf("Please enter either y or n\n");
    playAgain(field);
    return 0;
  }
}
