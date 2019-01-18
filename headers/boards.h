#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#ifndef BOARDS
#define BOARDS

void randBoard(Board b, Ships *s, char n);
void genBoard(Board b, Ships s, char name);
void getShips(Ships *s); // loads lengths of ships from file ships.len

#endif
