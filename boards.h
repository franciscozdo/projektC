#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "game.h"

#ifndef BOARDS
#define BOARDS

//void getBoard(Board b, Ships *s);
void randBoard(Board b, Ships *s, char n);
//void loadFromFile(Board b, Ships);
void genBoard(Board b, Ships *s, char name);

#endif
