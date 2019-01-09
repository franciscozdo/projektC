#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "fifo.h"
#include "game.h"

void sendMove(PipesPtr pipes, Shoot s); // wysyła ruch
bool getMessage(PipesPtr pipes, char *m); // pobiera wiadomość m = "m"  lub "r"
void sendFeedback(PipesPtr pipes, Shoot s, Status stat);
