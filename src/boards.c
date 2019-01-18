#include "../headers/boards.h"
#include "../headers/game.h"
#include <stdio.h>
// W grze nie jest za dużo statków więc jest bardzo mała szansa na to
// że funkcja nigdy się nie skończy wykonywać
// (gdy nie ma możliwości położenia jakiegoś statku na planszy po prostu
//  zaczyna układać statki od nowa)

static int min (int a, int b)
{
    return a < b ? a : b;
}

void genBoard(Board b, Ships s, char name)
{
    srand(time(0) * name);
    clearBoard(b);
    bool placed;
    //s->longest = 5;
    for (int i = s.longest; i > 0;) {
        for (int k = 0; k < s.count[i]; ++k) {
            int start = rand() % 100;
            int ind = start;
            placed = false;
            while (++ind % 100 != start) {
                int r = rand() % 2;
                Shoot pos = makeShoot(ind % 10, ind / 10);
                if (placeShip(i, r, pos, b) || placeShip(i, r ^ 1, pos, b)) {
                    placed = true;
                    break;
                } 
            }
            if (!placed)
                goto niedasie;
        }
        niedasie:
        if (!placed) {
            i = s.longest;
            clearBoard(b);
        } else
            i--;
    }
}

void randBoard(Board b, Ships *s, char n) 
{
    srand(time(0) * n);
    int ind = rand() % 9;
    getShips(s);
    genBoard(b, *s, n * ind);
}

void getShips(Ships *s)
{
    FILE *src = fopen("data/ships.len", "r");
    int c;
    int max[6] = {0, 5, 4, 3, 2, 2};
    s->longest = 0;
    while ((c = getc(src)) =='#')
        while ((c = getc(src)) != '\n');
    if (c < '9' && c >'0') ungetc(c, src);
    for (int i = 0; i < 5; ++i) {
        if (fscanf(src, "%d", &(s->count[i + 1]) ) != 0) {
            s->longest = i + 1;
            s->count[i + 1] = min(s->count[i + 1], max[i + 1]);
        }
    }
    fclose(src);
}
