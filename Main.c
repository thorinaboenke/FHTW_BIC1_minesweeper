/** \file
* \brief This file contains the main program for a minesweeper game */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "setup.h"
#include "gamestatistics.h"

// function prototypes
int check_all_cells_opened(Cell **field);
int check_all_mines_flagged(Cell **field);
void guess(Cell **field);
int lost(Cell **field);
void open_cell(int x, int y, Cell **field);
int play_again(Cell **field);
void print_field(Cell **field);
void reveal_all_mines(Cell **field);

/**
 Plays minesweeper

 Determines the gridsize and numer of mines from the command line arguments passed to the program.
 Defaults to 10x10 cells and 25 mines.
 Supports grid sizes between 5x5 and 24x24.
 Number of mines needs to be > 2, but not higher than half the number of cells.
 Attempts to load statistics with load_statistics().
 Generates a new playing field with generate_field().
 Prints the initial field with print_field().
 While neither the winning conditions are fulfilled (check_all_cells_opened() and check_all_mines_flagged()) and the player has not lost/hit a mine lost() the program prompts the player to guess a field and prints the new state of the field to the screen with print_field().
 Upon exiting this loop, the player is promted if they want to play again and the outer loop starts again from load_statistics() OR the program exits.

 */
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
    load_statistics();
    srand(time(NULL));
    field = generate_field();
    print_field(field);
    while (check_all_cells_opened(field) == 0 && check_all_mines_flagged(field) == 0 && lost(field) == 0){
      guess(field);
      print_field(field);
    }
  } while (play_again(field)==1);
  return 0;
}


/**
 Prints the playing field.

 Prints the playing field (a nested array of cells) with X (letters) and Y (numbers) coordinates.
 Depending on the flags set on a cell, the following output is printed:
 is_flagged == 1 : '?'
 is_opened == 0 : '-'
 is_opened == 1 && mine == 0 : '<number of adjacent mines'
 is_opened == 1 && mine == 1 : 'M'

 \param field a pointer to a nested array of cells
 */
void print_field(Cell **field){
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
      if(field[i][j].is_flagged == 1 && field[i][j].is_opened == 0) {
        printf("?\t");
      }
      else if(field[i][j].is_opened == 1 ){
        if(field[i][j].is_mine == 1){
          printf("M\t");
        }
        else if (field[i][j].adjacent_mines == 0){
          printf(" \t");
        } else {
        printf("%d\t", field[i][j].adjacent_mines);}
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
void guess(Cell **field){
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
  else if (str[0]== '?' && field[x][y].is_opened == 0){
    field[x][y].is_flagged = 1;
  }
  // prompts player again for input if input cell was already opened
  else if (field[x][y].is_opened == 1){
    printf("Please target an unopened cell.\n");
    guess(field);
  }
  // reveals all mines if a mine was hit
  else if (field[x][y].is_mine == 1) {
    reveal_all_mines(field);
    printf("Oh No. You hit a mine. You lost.\n");
    statistics[3]++; // cells opened
    statistics[2]++; // games lost
  } else {
    open_cell(x, y , field);
    statistics[3]++; // cells opened
  }
}

/**
 Opens a cell

 Sets is_open to 1 and is_flagged to 0.
 In case of no neighbouring mines, calls itself recursively for all neighbour cells with 0 neighbouring mine.
 \param  i integer x coordinate of cell to reveal
 \param  j integer y coordinate of cell to reveal
 \param field a pointer to a nested array of cells
 */
void open_cell(int i, int j, Cell **field){
  if (field[i][j].is_opened == 1) {
    return;
  }
  field[i][j].is_opened = 1;
  field[i][j].is_flagged = 0;
  // recursively call open_cell for all neighbour cells with 0 neighbouring mines
  if (field[i][j].adjacent_mines == 0) {
    if (j-1 >= 0 && field[i][j-1].adjacent_mines == 0) {
      open_cell(i,j-1, field);
      }
    if (i-1 >= 0 && field[i-1][j].adjacent_mines == 0) {
      open_cell(i-1,j, field);
    }
    if (i+1 < grid_size && field[i+1][j].adjacent_mines == 0) {
      open_cell(i+1,j, field);
    }
    if (j+1 < grid_size && field[i][j+1].adjacent_mines == 0) {
      open_cell(i,j+1, field);
    }
  }
}

/**
 Checks if all none mine cells are opened
If all non-mine cells were opened , prints a winning massage and updates statistics.

 \param field a pointer to a nested array of cells

 \return integer indicating if all cells are opened (1) or not (0)

 */
int check_all_cells_opened(Cell **field){
  for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].is_mine == 0 && field[i][j].is_opened == 0) {
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
int check_all_mines_flagged(Cell **field){
  int count = 0;
  int flagCount = 0;
  for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].is_mine == 1 && field[i][j].is_flagged == 1) {
        count++;
      }
      if (field[i][j].is_flagged == 1) {
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

 Checks for every cell if both mine and is_opened are 1.

 \param field a pointer to a nested array of cells

 \return result indicating if the game is lost (1) or not (0)

 */
int lost(Cell **field){
  for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].is_mine == 1 && field[i][j].is_opened == 1) {
        return 1;
      }
    }
  }
  return 0;
}

/**
 Reveals all mines.

 Set is_opened to 1 for every cell containing a mine.

 \param field a pointer to a nested array of cells

 */
void reveal_all_mines(Cell **field) {
   for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].is_mine == 1) {
        field[i][j].is_opened = 1;
      }
    }
  }
}

/**
 Restarts or ends game.

Prompts player if they want to play again.
Yes: calls save_statistics(), frees the memory for the current playing field.
No: calls save_statistics(), frees the memory for the current playing field, exits the program.

 \param field a pointer to a nested array of cells

 */

int play_again(Cell **field){
char input[2];
 printf("Would you like to play again? y/n \n");
 scanf("%c", &input[0]);
 fflush(stdin);
  if(input[0] == 'y' ||input[0] == 'Y'){
    save_statistics();
    printf("Ok, let's play again. \n");
    for (int i = 0; i < grid_size; i++){
      free(field[i]);
    }
    free(field);
    return 1;
  }
  else if (input[0] == 'n' || input[0] == 'N' ) {
    save_statistics();
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
    play_again(field);
    return 0;
  }
}
