#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


// function prototypes
struct cell** generateField();
void printField(struct cell** field);
void placeMine(struct cell** field);
void openFirstCell(struct cell** field);
void guess(struct cell** field);
void openCell(int x, int y, struct cell** field);
void revealAllMines(struct cell** field);
int lost(struct cell** field);
int playAgain(struct cell** field);
void loadStatistics();
void saveStatistics();
void printStatistics();
int allOpened(struct cell** field);
int allFlagged(struct cell** field);

// define cell struct
struct cell{
  int mine;
  int adjacent_mines;
  int isOpened;
  int isFlagged;
};

// global variables
struct cell** field;
char name[25];
int statistics[4]={0,0,0,0};
int grid_size;
int no_of_mines;


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
    loadStatistics();
    srand(time(NULL));
    field = generateField();
    printField(field);
    while (allOpened(field) == 0 && allFlagged(field) == 0 && lost(field) == 0){
      guess(field);
      printField(field);
    }
  } while (playAgain(field)==1);
  return 0;
}

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
  // initialize with default values
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

  // randomly open one field that is not a mine
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

void printField(struct cell** field){
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

void guess(struct cell** field){
  printf("Enter the cell you want to target in the format A2 and hit Enter.\n");
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
    guess(field);
  }
  else if (str[0]== '?' && field[x][y].isOpened == 0){
    field[x][y].isFlagged = 1; // flag mine
  }
  else if (field[x][y].isOpened == 1){
    printf("Please target an unopened cell.\n");
    guess(field);
  }
  else if (field[x][y].mine == 1) {
    revealAllMines(field);
    printf("Oh No. You hit a mine. You lost.\n");
    statistics[3]++; // cells opened
    statistics[2]++; // games lost
  } else {
    openCell(x, y , field);
    statistics[3]++; // cells opened
  }
};

void openCell(int i, int j, struct cell** field){
  if (field[i][j].isOpened == 1) {
    return;
  }
  field[i][j].isOpened = 1;
  field[i][j].isFlagged = 0;
  // recursively call openCell for all neighbour cells with 0 neighbouring mines
  if (field[i][j].adjacent_mines == 0) {
    if (j-1 >= 0 && field[i][j-1].adjacent_mines == 0) {
      openCell(i,j-1, field);
      }
    if (i-1 >= 0 && field[i-1][j].adjacent_mines == 0) {
      openCell(i-1,j, field);
    }
    if (i+1 < grid_size && field[i+1][j].adjacent_mines == 0) {
      openCell(i+1,j, field);
    }
    if (j+1 < grid_size && field[i][j+1].adjacent_mines == 0) {
      openCell(i,j+1, field);
    }
  }
}

int allOpened(struct cell** field){
  for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].mine == 0 && field[i][j].isOpened == 0) {
        return 0;
      }
    }
  }
  printf("All cells are opened.\n");
  printf("You won!\n");
  statistics[0]++; // games played
  return 1;
}
int allFlagged(struct cell** field){
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
  printf("There are %d mines.\n", no_of_mines - flagCount);
  if(count == no_of_mines) {
    printf("All mines flagged!\n");
    printf("You won!\n");
    statistics[1]++; // games won
    }
  return result;
}

int lost(struct cell** field){
  for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].mine == 1 && field[i][j].isOpened == 1) {
        return 1;
      }
    }
  }
  return 0;
}

void revealAllMines(struct cell** field) {
   for (int i = 0; i < grid_size; i++){
    for (int j = 0; j < grid_size; j++){
      if (field[i][j].mine == 1) {
        field[i][j].isOpened = 1;
      }
    }
  }
}


int playAgain(struct cell** field){
char input[2];
 printf("Would you like to play again? y/n \n");
 scanf("%c", &input[0]);
 fflush(stdin);
  if(input[0] == 'y' ||input[0] == 'Y'){
    saveStatistics();
    printf("Ok, let's play again. \n");
    for (int i = 0; i < grid_size; i++){
      free(field[i]);
    }
    free(field);
    return 1;
  }
  else if (input[0] == 'n' || input[0] == 'N' ) {
    saveStatistics();
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
    playAgain(field);
    return 0;
  }
}

void loadStatistics(){
  char filename[25];
  printf("Please enter your name (20 characters max.):\n");
  fgets(filename, 21, stdin);
  //fgets puts a newline character, so remove it first
  int len = strlen(filename);
  if(filename[len-1] == '\n'){
    filename[len-1] = 0;
    }
  // copy name to global variable name
  strcpy(name, filename);
  strcat(filename, ".txt");
  FILE* f;
  if((f = fopen(filename, "r")) == NULL){
      fprintf(stderr, "Cannot open statistics. Maybe you have not played before, or check for spelling mistakes.\n");
      return;
    }
      for(int i=0; i<4; i++){
          if(fscanf(f, "%d", &statistics[i]) != 1){
              fprintf(stderr, "Error reading from file\n");
              exit(1);
          }
      }
  if (fflush(f) != 0) {
    fprintf(stderr,"fflush was not successful!\n");
    exit(1);
  }
  if (fclose(f) != 0) {
    fprintf(stderr,"fclose was not successful!\n");
    exit(1);
  }
  printStatistics();
};

void saveStatistics(){
  statistics[0]++; // games played
  printf("Saving statistics.\n");
  char filename[25];
  strcpy(filename, name);
  strcat(filename, ".txt");

  FILE* f;
  if((f = fopen(filename, "w")) == NULL){
    fprintf(stderr, "Could not write to file");
    return;
  }
  for(int j=0; j<4; j++){
    fprintf(f, "%d\t", statistics[j]);
  }
  if (fflush(f) != 0) {
    fprintf(stderr,"fflush was not successful!\n");
    return;;
  }
  if (fclose(f) != 0) {
    fprintf(stderr,"fclose was  not successful!\n");
    return;
  }
};

void printStatistics(){
  printf("Player %s\n", name);
  printf("Your statistics:\n");
  printf("Games played: %d\n", statistics[0]);
  printf("Games won: %d\n", statistics[1]);
  printf("Games lost: %d\n", statistics[2]);
  printf("Opened cells: %d\n", statistics[3]);
};