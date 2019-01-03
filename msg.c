#include "msg.h"

void sendMove(PipesPtr pipes, Shoot s) 
{
	char msg[4];
	msg[0] = 's'; // wysyłam strzał
	msg[1] = 'A' + s.x;
	msg[2] = 'A' + s.y;
	msg[3] = '\0';
	printf("Wysyłam %s\n", msg);
}

void sendFeedback(PipesPtr pipes, Shoot s, Status stat)
{
	char msg[5];
	msg[0] = 'f'; // wysyłam wiadomość zwrotną
	msg[1] = 'A' + s.x;
	msg[2] = 'A' + s.x;
	msg[3] = stat;
	msg[4] = '\0';
	printf("Wysyłam %s\n", msg);
	sendStringToPipe(pipes, msg);
}
/*
void getMessage(PipesPtr pipes, char *m)
{
	// Tutaj wydaje mi się że coś nie bangla
	// bo czasem może przeczytać za dużo znaków
	// trzebaby czytać po jednym znaku i wstawiać jakieś zakończenie
	char msg[4];
	getStringFromPipe(pipes, msg, 4);
}
*/
