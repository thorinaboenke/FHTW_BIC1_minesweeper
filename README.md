FH Technikum BIC 1. Semester, Hardwarenahe Softwareentwicklung
Thorina Boenke 

# Minesweeper
## Compilation
gcc -std=c99 -Wall -Wextra -pedantic -Wno-unused-parameter -Iinclude Main.c Gamestatistics.c Setup.c -o minesweeper

## Execution
```
./minesweeper

./minesweeper <grid size>x<grid size> <mine count>

./minesweeper 10x10 20
```

The program accepts two command line arguments to define the size of a square grid (5x5 - 24x24) and mine count, min. 5 mines but not more than half the number of cells on the grid. The default values are 10x10 grid with 20 mines.

## Gameplay
A cell is revealed by entering its coordinates in the format 'A2', i.e. capital letter followed by a number.
A cell is flagged by entering its coordinates with a preceding '?', i.e. '?A2'.
Flagged cells can still be revealed later.
Additional characters entered after the coordinates will be ignored.

The player wins when 
- all cells without mines are revealed
- all mines are flagged

The player looses when
- a mine is revealed

## Statistics
The player is prompted for their name in the beginning and number of played games, lost games, won games and revealed cells are saved in a .txt file named after the player.
The cell revealed randomly in the beginning is not counted.

### Completed tasks
- [x] Main Task (30 points)
- [x] Task (15 points) - Extend the program to handle the case when a cell contains a zero in a different way
- [x] Task (15 points) - Extend the program to handle program arguments for a square Matrix size and amount of mines
- [x] Task (15 points) - Extend the program that a player can mark a mine with a prefix
- [x] Task (15 points) - Ask and save the names of the players. Generate a statistic for this player.
- [ ] Task (15 points) - Allow saving and continuing mid-game
- [ ] Task (25 points) - use n curses
