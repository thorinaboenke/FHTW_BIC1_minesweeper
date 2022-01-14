/** \file
* \brief This file handles setting up a new playfield for minesweeper*/

#include <stdio.h>
#include <stdlib.h>
#include "setup.h"

// global variables
struct cell** field;
int grid_size;
int no_of_mines;

/**
 Generates new playing field.

 Allocates memory dynamically for a nested array of Cells according to grid size.
 Initializes all flags of the cell structs with 0.
 Places mines randomly on the field with placeMine().
 Determines the number of adjacent mines for each cell.
 Randomly opens one cell that is not a mine with openFirstCell().

 @returns pointer to a nested array of cells ( i.e. the newly generated field)

 */

struct cell** generateField(){
// dynamically allocate memory for a nested array of Cell according to grid size

  int g = grid_size, i, j;

  struct cell** field = (struct cell**)malloc(g * sizeof(struct cell*));
  if (field == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(1);
  }
  for (i = 0; i < g; i++){
    field[i] = (struct cell*)malloc(g * sizeof(struct cell));
    if (field[i] == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(1);
  }
  }
  // initialize with default values
  for (i = 0; i < g; i++){
    for (j = 0; j < g; j++){
      field[i][j] = (struct cell) {.mine = 0, .adjacent_mines = 0, .isOpened = 0, .isFlagged = 0};
    }
  }

  // randomly distribute no_of_mines
  for (i = 0; i < no_of_mines; i++){
    placeMine(field);
  }

  // determine and save number of adjacent mines
  // always check before that index is not out of bounds
  for (int i = 0 ; i < grid_size; i++ ){
    for (int j = 0 ; j < grid_size; j++ ){
      if (field[i][j].mine == 1) {
        continue;
      }
      if (j-1 >= 0) {
        if (field[i][j-1].mine == 1){
        field[i][j].adjacent_mines +=1;
        }
      }
      if (j+1 < grid_size) {
        if (field[i][j+1].mine == 1) {
          field[i][j].adjacent_mines +=1;
        }
      }
      if (i+1 < grid_size) {
        if (field[i+1][j].mine == 1) {
          field[i][j].adjacent_mines +=1;
        }
      }
      if (i-1 >= 0) {
        if (field[i-1][j].mine == 1) {
          field[i][j].adjacent_mines +=1;
        }
      }
      if (i+1 < grid_size && j+1 < grid_size){
        if (field[i+1][j+1].mine == 1) {
          field[i][j].adjacent_mines +=1;
        }
      }
      if (i-1 >= 0 && j-1 >= 0) {
        if (field[i-1][j-1].mine == 1) {
          field[i][j].adjacent_mines +=1;
        }
      }
      if (i-1 >= 0 && j+1 < grid_size) {
        if (field[i-1][j+1].mine == 1) {
          field[i][j].adjacent_mines +=1;
        }
      }
      if (i+1 < grid_size && j-1 >= 0) {
        if (field[i+1][j-1].mine == 1) {
          field[i][j].adjacent_mines +=1;
        }
      }
    }
  }

  // randomly open one cell that is not a mine
  openFirstCell(field);
  return field;
};


/**
 Places a mine on the field.

 Generates random x and y coordinates.
 Checks that there is no mine on that cell yet and places a mine, else calls itself recursively until a free cell is targeted.
 (max. half the number of cells will contain a mine, so the recursion will eventually end)

 */
void placeMine(struct cell** field){
  int x = rand() % grid_size;
  int y = rand() % grid_size;
  if (field[x][y].mine == 0) {
    field[x][y].mine = 1;
  } else {
    placeMine(field);
  }
}

/**
 Opens the first cell.

 Generates random x and y coordinates.
 Checks that there is no mine on that cell and opens it, else calls itself recursively until a cell is targeted that does not contain a mine.
 (max. half the number of cells will contain a mine, so the recursion will eventually end)

 */
void openFirstCell(struct cell** field){
  int x = rand() % grid_size;
  int y = rand() % grid_size;
  if (field[x][y].mine != 1 && field[x][y].adjacent_mines != 0) {
    field[x][y].isOpened = 1;
  } else {
    openFirstCell(field);
  }
}