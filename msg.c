#include "msg.h"

void sendMove(PipesPtr pipes, Shoot s) 
{
	char msg[4];
	msg[0] = 's'; // wysyłam strzał
	msg[1] = 'A' + s.x;
	msg[2] = 'A' + s.y;
	msg[3] = '\0'; //chyba nie będzie potrzebne, bo będziemy od razu wiedzieć którą wiadomość dostajemy
	
	sendStringToPipe(pipes, msg);
	printf("Wysyłam %s\n", msg);
}

void sendFeedback(PipesPtr pipes, Shoot s, Status stat)
{
	char msg[5];
	msg[0] = 'f'; // wysyłam wiadomość zwrotną
	msg[1] = 'A' + s.x;
	msg[2] = 'A' + s.x;
	msg[3] = stat;
	msg[4] = '\0'; //chyba nie będzie potrzebne, bo będziemy od razu wiedzieć którą wiadomość dostajemy
	
	sendStringToPipe(pipes, msg);
	printf("Wysyłam %s\n", msg);
}

bool getMessage(PipesPtr pipes, char *msg)
{
	char msgg[50];
	char mode;
	if (getStringFromPipe(pipes, msgg, 50)) {
		strcpy(msg, msgg);
		printf("przeczytałem: %s\n", msg);
		return true;
	}
	return false;
}
