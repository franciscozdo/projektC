#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

/********************************************
 *            Franciszek Zdobylak           *
 *              nr ind. 310313              *
 *                                          *
 *   Projekt na Wstęp do Programowania w C  *
 *               Gra w Statki               *
 *                                          *
 ********************************************/

#ifndef BOARDS
#define BOARDS

void randBoard(Board b, Ships *s, char n);
void genBoard(Board b, Ships s, char name);
void getShips(Ships *s); // loads lengths of ships from file ships.len

#endif
