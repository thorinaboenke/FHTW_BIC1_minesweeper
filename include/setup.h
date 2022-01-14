#ifndef SETUP_H
#define SETUP_H

extern int grid_size;
extern int no_of_mines;

typedef struct cell {
  int mine;
  int adjacent_mines;
  int isOpened;
  int isFlagged;
} Cell;

extern Cell** field;

//FUNCTION PROTOTYPES
Cell** generateField();
void placeMine(Cell** field);
void openFirstCell(Cell** field);

#endif