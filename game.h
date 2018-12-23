#include <stdbool.h>

typedef enum {NOT_SHOOT = 0, MISSED = 6, HIT = 7, UNKNOWN = 8} Status; // 1 - 5 lengths of battleships hit

typedef struct shoot {
	int x, y;
} Shoot;

typedef int Board[10][10]; // to store information about status

void clearBorad(Board b);
Shoot makeShoot(int x, int y);
bool checkShoot(Shoot s, Board b); // checks if it's a proper move
void markOnBoard(Shoot s, Board b, Status stat);
int checkOnBoard(Shoot s, Board b);
