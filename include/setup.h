#ifndef SETUP_H
#define SETUP_H

extern int grid_size;
extern int no_of_mines;

struct cell {
  int mine;
  int adjacent_mines;
  int isOpened;
  int isFlagged;
};

extern struct cell** field;

//FUNCTION PROTOTYPES
struct cell** generateField();
void placeMine(struct cell** field);
void openFirstCell(struct cell** field);

#endif