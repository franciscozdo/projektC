#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "fifo.h"
#include "game.h"

/********************************************
 *            Franciszek Zdobylak           *
 *              nr ind. 310313              *
 *                                          *
 *   Projekt na Wstęp do Programowania w C  *
 *               Gra w Statki               *
 *                                          *
 ********************************************/

void sendMove(PipesPtr pipes, Shoot s);
bool getMessage(PipesPtr pipes, char *m);
void sendFeedback(PipesPtr pipes, Shoot s, Status stat);
void sendSignal(PipesPtr pipes, int signal); // 1 - new game, 2 - give up
void sendReveal(PipesPtr pipes, Shoot s, Status stat);
