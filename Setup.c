
#include <stdio.h>
#include <stdlib.h>
#include "setup.h"

// global variables
struct cell** field;
int grid_size;
int no_of_mines;

struct cell** generateField(){
//dynamically allocate memory for a nested array of fields according to grid size

  int g = grid_size, i, j;

  struct cell** field = (struct cell**)malloc(g * sizeof(struct cell*));
  if (field == NULL){
    fprintf(stderr, "Out of memory\n");
    exit(1);
  }
  for (i = 0; i < g; i++){
    field[i] = (struct cell*)malloc(g * sizeof(struct cell));
    if (field[i] == NULL){
    fprintf(stderr, "Out of memory\n");
    exit(1);
  }
  }
  //initialize with default values
  for (i = 0; i < g; i++){
    for (j = 0; j < g; j++){
      field[i][j] = (struct cell) {.mine= 0, .adjacent_mines = 0, .isOpened = 0, .isFlagged = 0};
    }
  }

  // randomly distribute no_of_mines
  for (i = 0; i < no_of_mines; i++){
    placeMine(field);
  }

  // save number of adjacent mines
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

  //randomly open one field that is not a mine
  openFirstCell(field);
  return field;
};

void placeMine(struct cell** field){
  int x = rand() % grid_size;
  int y = rand() % grid_size;
  if (field[x][y].mine == 0) {
    field[x][y].mine = 1;
  } else {
    placeMine(field);
  }
}

void openFirstCell(struct cell** field){
  int x = rand() % grid_size;
  int y = rand() % grid_size;
  if (field[x][y].mine != 1 && field[x][y].adjacent_mines != 0) {
    field[x][y].isOpened = 1;
  } else {
    openFirstCell(field);
  }
}