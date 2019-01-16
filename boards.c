#include "boards.h"
#include "game.h"
#include <stdio.h>
// W grze nie jest za dużo statków więc jest bardzo mała szansa na to
// że funkcja nigdy się nie skończy wykonywać
// (gdy nie ma możliwości położenia jakiegoś statku na planszy po prostu
//  zaczyna układać statki od nowa)

void genBoard(Board b, Ships *s, char name)
{
    srand(time(0) * name);
    clearBoard(b);
    bool placed;
    //s->longest = 5;
    for (int i = s->longest; i > 0; --i) {
        for (int k = 0; k < s->count[i]; ++k) {
            int start = rand() % 100;
            int ind = start;
            placed = false;
            while (++ind % 100 != start) {
                int r = rand() % 2;
                if (placeShip(b, i, ind, r) || placeShip(b, i, ind, r ^ 1)) {
                    placed = true;
                    break;
                }
            }
        }
        if (!placed) {
            i = s->longest + 1;
            clearBoard(b);
        }
    }
}

void randBoard(Board b, Ships *s, char n) 
{
    //sleep(1);
    srand(time(0) * n);
    int ind = rand() % 9;
    /*for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            b[i][j] = p[ind][j][i];
        }
    }*/
    s->count[1] = 3;//3;
    s->count[2] = 3;//3;
    s->count[3] = 2;//2;
    s->count[4] = 1;//1;
    s->count[5] = 1;//1;
    s->longest = 5;//5;
    genBoard(b, s, n * ind);
}
