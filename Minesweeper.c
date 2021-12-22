#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct cell** generateField(int grid_size, int no_of_mines);
void printField(int grid_size, struct cell** field);
void placeMine(int grid_size, struct cell** field);
void openFirstCell(int grid_size, struct cell** field);
void guess(struct cell** field, int grid_size);
void openCell(int x, int y, struct cell** field, int grid_size);
void flagMine(int x, int y, struct cell** field);
void revealAllMines(int grid_size, struct cell** field);
void mine(int grid_size, struct cell** field);
void gameOver(struct cell** field);
void win(struct cell** field, int grid_size);
void lost();
void playAgain(struct cell** field, int grid_size);
void gameOver();
void loadStatistics();
void saveStatistics();
void printStatistics();
int allOpened(struct cell** field, int grid_size);
int allFlagged(struct cell** field, int grid_size, int no_of_mines);

// define cell struct
struct cell{
  int mine;
  int adjacent_mines;
  int isOpened;
  int isFlagged;
};

int play = 1;
struct cell** field;


int main(int argc, char *argv[]){
  int grid_size = 5;
  int no_of_mines = 1;
  //use command line arguments for grid size and mine count, if given
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
    if(mines < 5){
    printf("Please supply a mine count of at least 5 as second command line argument, but not higher than half the number of cells.\n");
      exit(1);
    }
    if(mines > ((grid * grid)/2)){
    printf("Please supply a mine count of at least 5 as second command line argument, but not higher than half the number of cells.\n");
      exit(1);
    }
    no_of_mines = mines;
  }
  while (play == 1 ){
    srand(time(NULL));  //seed for random generator
    field = generateField(grid_size, no_of_mines);
    printField(grid_size, field);
    while (!allOpened(field, grid_size)){
    guess(field, grid_size);
    printField(grid_size, field);
    if (allOpened(field, grid_size)==1){
      break;
    }
     if (allFlagged(field, grid_size, no_of_mines)==1){
      break;
    }
  }
  win(field, grid_size);
  }

return 0;
}

struct cell** generateField(int grid_size, int no_of_mines){
//dynamically allocate memory for a nested array of fields according to grid size

  int g = grid_size, i, j, count;

  struct cell** field = (struct cell**)malloc(g * sizeof(struct cell*));
  for (i = 0; i < g; i++){
    field[i] = (struct cell*)malloc(g * sizeof(struct cell));
  }
  //initialize with default values
  for (i = 0; i < g; i++){
    for (j = 0; j < g; j++){
      field[i][j] = (struct cell) {.mine= 0, .adjacent_mines = 0, .isOpened = 0, .isFlagged = 0};
    }
  }

  // randomly distribute no_of_mines
  for (i = 0; i < no_of_mines; i++){
    placeMine(grid_size, field);
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
  openFirstCell(grid_size, field);
  return field;
};

void placeMine(int grid_size, struct cell** field){
  int x = rand() % grid_size;
  int y = rand() % grid_size;
  if (field[x][y].mine == 0) {
    field[x][y].mine = 1;
  } else {
    placeMine(grid_size, field);
  }
}

void openFirstCell(int grid_size, struct cell** field){
  int x = rand() % grid_size;
  int y = rand() % grid_size;
  if (field[x][y].mine != 1 && field[x][y].adjacent_mines != 0) {
    field[x][y].isOpened = 1;
  } else {
    openFirstCell(grid_size, field);
  }
}

void printField(int grid_size, struct cell** field){
  // print x coordinates
  printf(" \t");
  for (int i = 0 ; i < grid_size; i++ ){
    printf("%c", i+65);
    printf(" \t");
  }
  printf(" \n");
  // print  field
  // print x coordinated
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

void guess(struct cell** field, int grid_size ){
  printf("Enter the cell you want to target in the format A2 (for example) and hit Enter.\n");
  char str[5];
  fgets(str, 5, stdin);
  fflush(stdin);
  int x;
  int y;

  y = (int)str[0]-65;
  x = atoi(&str[1])-1;

if(str[0]== '?'){
  y = (int)str[1]-65;
  x = atoi(&str[2])-1;
}
  if (x < 0 || y < 0 || x > grid_size-1 || y > grid_size-1) {
    printf("Please target an existing cell.\n");
    guess(field, grid_size);
  }
  else if (str[0]== '?' && field[x][y].isOpened == 0){
    flagMine(x, y, field);
  }
  else if (field[x][y].isOpened == 1){
    printf("Please target an unopened cell.\n");
    guess(field, grid_size);
  }
  else if (field[x][y].mine == 1) {
    mine(grid_size, field);
  } else {
    openCell(x, y , field, grid_size);
  }
};

void flagMine(int x, int y, struct cell** field) {
  field[x][y].isFlagged = 1;
  }

void openCell(int i, int j, struct cell** field, int grid_size ){
  if (field[i][j].isOpened == 1) {
    return;
  }
  field[i][j].isOpened = 1;
  field[i][j].isFlagged = 0;
  // recursively call openCell for all neighbour cells with 0 neighbouring mines
  if (field[i][j].adjacent_mines == 0) {
    if (j-1 >= 0 && field[i][j-1].adjacent_mines == 0) {
      openCell(i,j-1, field, grid_size);
      }
    if (i-1 >= 0 && field[i-1][j].adjacent_mines == 0) {
      openCell(i-1,j, field, grid_size);
    }
    if (i+1 < grid_size && field[i+1][j].adjacent_mines == 0) {
      openCell(i+1,j, field, grid_size);
    }
    if (j+1 < grid_size && field[i][j+1].adjacent_mines == 0) {
      openCell(i,j+1, field, grid_size);
    }
  }
}
int allOpened(struct cell** field, int grid_size){
  for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].mine == 0 && field[i][j].isOpened == 0) {
        return 0;
      }
    }
  }
  return 1;
}
int allFlagged(struct cell** field, int grid_size, int no_of_mines){
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
  printf("There are %d mines remaining.\n", no_of_mines - flagCount);
  if(count == no_of_mines) {
    printf("All mines flagged!");
    }
  return result;
}

void lost(){
  printf("You lost. Saving statistics.\n");
};

void mine(int grid_size, struct cell** field){
  revealAllMines(grid_size, field);
  printField(grid_size, field);
  printf("Oh No. You hit a mine.\n");
  lost();
  playAgain(field, grid_size);
};

void revealAllMines(int grid_size, struct cell** field) {
   for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].mine == 1) {
        field[i][j].isOpened = 1;
      }
    }
  }
}

void win(struct cell** field, int grid_size){
  printf("You won. Saving statistics.\n");
  playAgain(field, grid_size);
};

void playAgain(struct cell** field, int grid_size){
char input[2];
 printf("Would you like to play again? y/n \n");
 scanf("%c", &input[0]);
 fflush(stdin);
  if(input[0] == 'y' ||input[0] == 'Y'){
    printf("Ok, let's play again. \n");
    field = generateField(grid_size, no_of_mines);
  }
  else if (input[0] == 'n' || input[0] == 'N' ) {
    printf("Ok, goodbye. \n");
    for (int i = 0; i < grid_size; i++){
    free(field[i]);
    }
    free(field);
    play = 0;
  }
  else {
    printf("Please enter either y or n\n");
    playAgain(field, grid_size);
  }
};

void loadStatistics(){};
void saveStatistics(){};
void printStatistics(int statistics[]){
  printf("Player Susi");
  printf("Your statistics:\n");
  printf("Games played: %d\n", statistics[0]);
  printf("Games won: %d\n", statistics[1]);
  printf("Games lost: %d\n", statistics[2]);
  printf("Opened cells: %d\n", statistics[3]);
};