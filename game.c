#include "game.h"

Shoot makeShoot(int a, int b)
{
	Shoot s;
	s.x = a;
	s.y = b;
	return s;
}

void clearBorad(Board b)
{
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			b[i][j] = NOT_SHOOT;
		}
	}
}

bool checkShoot(Shoot s, Board b)
{
	return b[s.x][s.y] == 0;
}

void markOnBoard(Shoot s, Board b, Status stat)
{
	b[s.x][s.y] = stat;
}

int checkOnBoard(Shoot s, Board b)
{
	return b[s.x][s.y];
}
