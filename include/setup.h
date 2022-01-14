#ifndef SETUP_H
#define SETUP_H

extern int grid_size;
extern int no_of_mines;


/**
 * \typedef
 a structure to represent one square (cell) on the playing field.
 The struct members indicate the state of the cell, i.e. if it contains a mine \c mine, how many mines are in the adjacent fields \c adjacentMines, if the cell has been revealed \c isOpened and if the cell has ben flagged \c isFlagged
 */
typedef struct cell {
  int mine; /**< if the cell contains a mine */
  int adjacentMines; /**< how many mines are in the adjacent fields */
  int isOpened; /**< has the cell been revealed */
  int isFlagged; /**< has the cell has ben flagged for potentially containing a mine*/
} Cell;

extern Cell** field;

//FUNCTION PROTOTYPES
Cell** generateField();
void placeMine(Cell** field);
void openFirstCell(Cell** field);

#endif