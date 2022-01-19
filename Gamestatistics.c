
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


void load_statistics(){
  char temp[256];
  char filename[30];
  printf("Please enter your name (20 characters max.):\n");
  fgets(temp, 256, stdin);
  strncpy(filename, temp, 20);
  filename[20] = '\0';
  // copy to global variable name, name is then used in save_statistics()
  strcpy(name, filename);
  strcat(filename, ".txt");
  FILE* f;
  if((f = fopen(filename, "r")) == NULL){
      fprintf(stderr, "Cannot open statistics. Maybe you have not played before, or check for spelling mistakes.\n");
      return;
    }
      for(int i=0; i<4; i++){
          if(fscanf(f, "%d", &statistics[i]) != 1){
            // for example if file does not contain integers
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
  print_statistics();
}

void save_statistics(){
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
}

void print_statistics(){
  printf("Player %s\n", name);
  printf("Your statistics:\n");
  printf("Games played: %d\n", statistics[0]);
  printf("Games won: %d\n", statistics[1]);
  printf("Games lost: %d\n", statistics[2]);
  printf("Opened cells: %d\n", statistics[3]);
}
