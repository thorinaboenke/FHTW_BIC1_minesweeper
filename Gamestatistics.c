
/** \file
* \brief This file handled loading and saving of player statistics */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gamestatistics.h"

/**
 The name entered by the player. Statistics will be loaded from/saved in a file <name>.txt
 */
char name[25];
/**
 Game statistics. loaded from/saved in a file <name>.txt. [0]played games, [1]lost games, [2]won games and [3]revealed cells
 */
int statistics[4]={0,0,0,0};

/**
 Loads player statistics.

 If a player has already played and statistics are saved in the same directory in a .txt file under their name they are read into statistics array and printed to the screen with printStatistics().
 */
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


/**
  Saves player statistics

  Saves player statistics from statistics array into .txt file under the players name
 */
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
    fprintf(stderr,"fclose was not successful!\n");
    return;
  }
};


/**
Prints player statistics

Prints player statistics (games played, games won, games lost, openend cells) to the screen, used by loadStatistics().

 */
void printStatistics(){
  printf("Player %s\n", name);
  printf("Your statistics:\n");
  printf("Games played: %d\n", statistics[0]);
  printf("Games won: %d\n", statistics[1]);
  printf("Games lost: %d\n", statistics[2]);
  printf("Opened cells: %d\n", statistics[3]);
};