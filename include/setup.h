#ifndef SETUP_H
#define SETUP_H

extern int grid_size;
extern int no_of_mines;


/**
 * \typedef
 a structure to represent one square (cell) on the playing field.
 The struct members indicate the state of the cell, i.e. if it contains a mine \c is_mine, how many mines are in the adjacent fields \c adjacent_mines, if the cell has been revealed \c check_all_cells_opened and if the cell has ben flagged \c check_all_mines_flagged
 */
typedef struct cell {
  int adjacent_mines; /**< how many mines are in the adjacent fields */
  int is_flagged; /**< has the cell has ben flagged for potentially containing a mine*/
  int is_mine; /**< if the cell contains a mine */
  int is_opened; /**< has the cell been revealed */
} Cell;

extern Cell** field;

//FUNCTION PROTOTYPES
Cell** generate_field();
void place_mine(Cell** field);
void open_first_cell(Cell** field);

#endif
