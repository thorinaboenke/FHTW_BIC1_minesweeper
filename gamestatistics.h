#ifndef GAMESTATISTICS_H
#define GAMESTATISTICS_H

extern int statistics[4];
//FUNCTION PROTOTYPES

/**
 Loads player statistics.

 Prompts player to enter their name. The first 20 characters are used for naming the file, all following characters are ignored.
 If a player has already played and statistics are saved in the same directory in a .txt file under their name, statistics are read into statistics array and printed to the screen with print_statistics().

 \note Can handle user input up to 256 characters.

 */
void load_statistics();

/**
  Saves player statistics

  Saves player statistics from statistics array into .txt file under the players name
 */
void save_statistics();

/**
Prints player statistics

Prints player statistics (games played, games won, games lost, openend cells) to the screen, used by load_statistics().

 */
void print_statistics();

#endif
