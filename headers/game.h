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
typedef struct ships {
   int count[6];
   int longest;
} Ships; // to store how many ships of specified length you have

void clearBoard(Board b);
Shoot makeShoot(int x, int y);
void markOnBoard(Shoot s, Board b, Status stat);
Status checkOnBoard(Shoot s, Board b);
bool isSunk(Shoot s, Board b);
int markSunk(Shoot s, Board b); // returns number of changed boxes
bool allSunk(Ships ships); // n - number of lengths of ships
void copyBoard(Board b1, Board b2);
bool placeShip(int slen, int orientation, Shoot pos, Board b);
int removeShip(Shoot pos, Board b);

#endif
