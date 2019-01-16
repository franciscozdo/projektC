#include "game.h"

static int dir[8] = {1, 0, -1, 0, 0, 1, 0, -1}; // directions for dfs in isSunk()
static int skos[8] = {1, 1, -1, -1, 1, -1, -1, 1};
// cheks if coordinates are in board (in range [0, 10])
static bool inBoard(Shoot s) 
{
    return s.x < 10 && s.x >= 0 && s.y < 10 && s.y >= 0;
}

Shoot makeShoot(int a, int b)
{
	Shoot s;
	s.x = a;
	s.y = b;
	return s;
}

/*Shoot makeShootFromStr(char *s) 
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
}*/

/*bool isCorrect(Shoot s) 
{
	return s.x < 10 && s.x >= 0 && s.y < 10 && s.y >= 0;
}*/	

void clearBoard(Board b)
{
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			b[i][j] = NOT_SHOOT;
		}
	}
}

void copyBoard(Board b1, Board b2)
{
    for (int i = 0; i < 100; ++i)
        b1[i/10][i%10] = b2[i/10][i%10];
}

void markOnBoard(Shoot s, Board b, Status stat)
{
	b[s.x][s.y] = stat;
}

int checkOnBoard(Shoot s, Board b)
{
	return b[s.x][s.y];
}

bool isSunk(Shoot s, Board b, bool vis[10][10])
{
    Status stat = checkOnBoard(s, b);
    //printf("spr %d %d - stat %d\n", s.x, s.y, stat);
    if (stat == SHIP)
        return false;
    //if (stat == MISSED || stat == NOT_SHOOT)
    //    return true;
    bool cond = true;
    vis[s.x][s.y] = true;
    for (int i = 0; i < 4; ++i) {
        Shoot ns = makeShoot(s.x + dir[2 * i], s.y + dir[2 * i + 1]);
        if (inBoard(ns) && !vis[ns.x][ns.y] && (
                    checkOnBoard(ns, b) == SHIP || checkOnBoard(ns, b) == HIT 
                    || checkOnBoard(ns, b) == MY_HIT))
            cond &= isSunk(ns, b, vis);
    }
    return cond;
}

int markSunk(Shoot s, Board b)
{
    if (checkOnBoard(s, b) != MY_HIT && checkOnBoard(s, b) != HIT)
        return 0;
    //printf("s%d\n", checkOnBoard(s, b));
    markOnBoard(s, b, SUNK);
    int n = 1; // counting actual place
    for (int i = 0; i < 4; ++i) {
        Shoot ns = makeShoot(s.x + dir[2 * i], s.y + dir[2 * i + 1]);
        //printf("ns%d\n", checkOnBoard(ns, b));
        if (inBoard(ns) && (checkOnBoard(ns, b) == MY_HIT || checkOnBoard(ns, b) == HIT))
            n += markSunk(ns, b);
    }  
    return n;
}

bool allSunk(Ships s)
{
    for (int i = 1; i <= s.longest; ++i) {
        //printf("[s%d - %d]", i, s.count[i]);
        if (s.count[i] != 0){//puts("");
            return false;}
    }//puts("");
    return true;
}

bool placeShip(Board b, int s, int ind, int orientation)
{
    int x = ind % 10;
    int y = ind / 10;
    if (orientation == 1) {
        if (x + s > 10) return false;
        for (int i = 0; i < s; ++i) {
            if (b[x + i][y] == 1) return false;
            for(int j = 0; j < 4; ++j) {
                if (x + i + dir[2 * j] >= 10 || x + i + dir[2 * j] < 0 ||
                        y + dir[2 * j + 1] >= 10 || y + dir[2 * j + 1] < 0 ||
                        x + i + skos[2 * j] >= 10 || x + i + skos[2 * j] < 0 ||
                        y + skos[2 * j + 1] >= 10 || y + skos[2 * j + 1] < 0)
                   continue; 

                if (b[x + i + dir[2 * j]][y + dir[2 * j + 1]] == 1)
                    return false;
                if (b[x + i + skos[2 * j]][y + skos[2 * j + 1]] == 1)
                    return false;
            }   
        }
        for (int i = 0; i < s; ++i) 
            b[x + i][y] = 1;
    } else {
        if (y + s > 10) return false;
        for (int i = 0; i < s; ++i) {
            if (b[x][y + i] == 1) return false;
            for (int j = 0; j < 4; ++j) {
                if (x + dir[2 * j] >= 10 || x + dir[2 * j] < 0 ||
                        y + i + dir[2 * j + 1] >= 10 || y  + i + dir[2 * j + 1] < 0 ||
                        x + skos[2 * j] >= 10 || x + skos[2 * j] < 0 ||
                        y  + i + skos[2 * j + 1] >= 10 || y + i + skos[2 * j + 1] < 0)
                   continue;
                if (b[x + dir[2 * j]][y + i + dir[2 * j + 1]] == 1)
                    return false;
                if (b[x + skos[2 * j]][y + i + skos[2 * j + 1]] == 1)
                    return false;
            }
        }
        for (int i = 0; i < s; ++i) 
            b[x][y + i] = 1;
    }
    return true;
}

int removeShip(Shoot pos, Board b)
{
    int len = 1;
    markOnBoard(pos, b, NOT_SHOOT);
    for (int i = 0; i < 4; ++i) {
        Shoot ns = makeShoot(pos.x + dir[2 * i], pos.y + dir[2 * i + 1]);
        if (checkOnBoard(ns, b) == SHIP)
            len += removeShip(ns, b);
    }
    return len;
}
