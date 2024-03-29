#include "../headers/game.h"

/********************************************
 *            Franciszek Zdobylak           *
 *              nr ind. 310313              *
 *                                          *
 *   Projekt na Wstęp do Programowania w C  *
 *               Gra w Statki               *
 *                                          *
 ********************************************/

static int dir[8] = {1, 0, -1, 0, 0, 1, 0, -1}; // directions for dfs in isSunk()
static int skos[8] = {1, 1, -1, -1, 1, -1, -1, 1};

static bool inBoard(Shoot s)
{
    return s.x >= 0 && s.x <= 9  && s.y >= 0 && s.y <= 9;
}

Shoot makeShoot(int a, int b)
{
    Shoot s;
    s.x = a;
    s.y = b;
    return s;
}

void clearBoard(Board b)
{
    for (int i = 0; i < 100; ++i)
        b[i%10][i/10] = NOT_SHOOT;
}

void copyBoard(Board dest, Board source)
{
    for (int i = 0; i < 100; ++i)
        dest[i%10][i/10] = source[i%10][i/10];
}

void markOnBoard(Shoot s, Board b, Status stat)
{
    b[s.x][s.y] = stat;
}

Status checkOnBoard(Shoot s, Board b) 
{
    return b[s.x][s.y];
}

bool isSunk(Shoot s, Board b)
{
    Status stat = checkOnBoard(s, b);
    if (stat == MISSED) return true;
    if (stat == SHIP)
        return false;
    bool cond = (stat == HIT || stat == MY_HIT);
    //printf("sprawdzam %d %d = %d\n", s.x, s.y, cond);
    markOnBoard(s, b, NOT_SHOOT); // marks to avoid returning in the same 
    
    for (int i = 0; i < 4; ++i) {
        Shoot ns = makeShoot(s.x + dir[2 * i], s.y + dir[2 * i + 1]);
        if (inBoard(ns) && checkOnBoard(ns, b) != NOT_SHOOT) {// && (checkOnBoard(ns, b) == HIT || checkOnBoard(ns, b) == MY_HIT))
            cond &= isSunk(ns, b);
        }
    }
    markOnBoard(s, b, stat); // marks previous value
    return cond;
}

int markSunk(Shoot s, Board b)
{
    if (checkOnBoard(s, b) != MY_HIT && checkOnBoard(s, b) != HIT)
        return 0;
    
    markOnBoard(s, b, SUNK);
    int n = 1; // counting actual place
    for (int i = 0; i < 4; ++i) {
        Shoot ns = makeShoot(s.x + dir[2 * i], s.y + dir[2 * i + 1]);
        if (inBoard(ns) && (checkOnBoard(ns, b) == MY_HIT || checkOnBoard(ns, b) == HIT))
            n += markSunk(ns, b);
    }  
    return n;
}

bool allSunk(Ships s)
{
    for (int i = 1; i <= s.longest; ++i)
        if (s.count[i] != 0)
            return false;
    return true;
}

int removeShip(Shoot pos, Board b)
{
    int len = 1;
    markOnBoard(pos, b, NOT_SHOOT);
    for (int i = 0; i < 4; ++i) {
        Shoot ns = makeShoot(pos.x + dir[2 * i], pos.y + dir[2 * i + 1]);
        if (inBoard(ns) && checkOnBoard(ns, b) == SHIP)
            len += removeShip(ns, b);
    } 
    return len;
}

bool placeShip(int slen, int orientation, Shoot pos, Board b)
{
   if (orientation == 1) {
        Shoot ns = makeShoot(pos.x + slen - 1, pos.y);
        if (!inBoard(ns)) 
            return false;

        for (int k = 0; k < slen; ++k) {
            if (checkOnBoard(makeShoot(pos.x + k, pos.y), b) != NOT_SHOOT)
                return false;
            for (int i = 0; i < 4; ++i) {
                Shoot ns = makeShoot(pos.x + k + dir[2 * i], pos.y + dir[2 * i  +1]);
                if (inBoard(ns) && checkOnBoard(ns, b) != NOT_SHOOT)
                    return false;
                ns = makeShoot(pos.x + k + skos[2 * i], pos.y + skos[2 * i + 1]);
                if (inBoard(ns) && checkOnBoard(ns, b) != NOT_SHOOT)
                    return false;
            }
        }
        for (int k = 0; k < slen; ++k) 
            markOnBoard(makeShoot(pos.x + k, pos.y), b, SHIP);
   } else {
        Shoot ns = makeShoot(pos.x, pos.y + slen - 1);
        if (!inBoard(ns)) 
            return false;

        for (int k = 0; k < slen; ++k) {
            if (checkOnBoard(makeShoot(pos.x, pos.y + k), b) != NOT_SHOOT)
                return false;
            for (int i = 0; i < 4; ++i) {
                Shoot ns = makeShoot(pos.x + dir[2 * i], pos.y + k + dir[2 * i  +1]);
                if (inBoard(ns) && checkOnBoard(ns, b) != NOT_SHOOT)
                    return false;
                ns = makeShoot(pos.x + skos[2 * i], pos.y + k + skos[2 * i + 1]);
                if (inBoard(ns) && checkOnBoard(ns, b) != NOT_SHOOT)
                    return false;
            }
        }
        for (int k = 0; k < slen; ++k) 
            markOnBoard(makeShoot(pos.x, pos.y + k), b, SHIP);
   } 
   return true;
}
