#include <stdbool.h>
#include <ctype.h>

#ifndef GAME
#define GAME


typedef enum {NOT_SHOOT = 0, SHIP = 1, MISSED =2, HIT = 3, SUNK = 4, MY_HIT = 5, UNKNOWN = 8} Status;
                                                            // ^if my ship is hit

typedef struct shoot {
	int x, y;
} Shoot;

typedef int Board[10][10]; // to store information about status

void clearBoard(Board b);
Shoot makeShoot(int x, int y);
//Shoot makeShootFromStr(char *s);
bool isCorrect(Shoot s);
//bool checkShoot(Shoot s, Board b); // checks if it's a proper move
void markOnBoard(Shoot s, Board b, Status stat);
int checkOnBoard(Shoot s, Board b);
bool isSunk(Shoot s, Board b, bool vis[10][10]);
int markSunk(Shoot s, Board b); // returns number of changed boxes

#endif
