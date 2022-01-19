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

/**
 Generates new playing field.

 Generates new playing field.
 Allocates memory dynamically for a nested array of cells according to grid size.
 Initializes all flags of the cell structs with 0.
 Places mines randomly on the field with placeMine().
 Determines the number of adjacent mines for each cell.
 Randomly opens one cell that is not a mine with open_first_cell().

 \returns pointer to a nested array of cells (i.e. the newly generated field)

 */
Cell** generate_field();

/**
 Places a mine on the field.

 Generates random x and y coordinates.
 Checks that there is no mine on that cell yet and places a mine with place_mine(), else calls itself recursively until a free cell is targeted.
 (max. half the number of cells will contain a mine, so the recursion will eventually end)

 \param field a pointer to a nested array of cells

 */
void place_mine(Cell** field);

/**
 Opens the first cell.

 Generates random x and y coordinates.
 Checks that there is no mine on that cell and opens it, else calls itself recursively until a cell is targeted that does not contain a mine.
 (max. half the number of cells will contain a mine, so the recursion will eventually end)

 \param field a pointer to a nested array of cells

 */
void open_first_cell(Cell** field);

#endif
