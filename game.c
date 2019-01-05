#include "game.h"

Shoot makeShoot(int a, int b)
{
	Shoot s;
	s.x = a;
	s.y = b;
	return s;
}

Shoot makeShootFromStr(char *s) 
{
	int x;
	if (islower(s[0])) 
		x = s[0] - 'a';
	else
		x = s[0] - 'A';
	int y = s[1] - '0';
	if (isdigit(s[2])){
		y *= 10;
		y += s[2] - '0';
	}
	y--; // Bo numeruję od zera
	return makeShoot(x, y);
}

bool isCorrect(Shoot s) 
{
	return s.x < 10 && s.x >= 0 && s.y < 10 && s.y >= 0;
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
