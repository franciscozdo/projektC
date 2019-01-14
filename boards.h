#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifndef BOARDS
#define BOARDS

void getBoard(Board b, int ship_counter[], int *longest);
void randBoard(Board b, int ship_counter[], int *longest, char n);
void loadFromFile(Board b, int ship_counter[]);

#endif
