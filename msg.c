#include "msg.h"
//#include <stdio.h>

void sendMove(PipesPtr pipes, Shoot s) 
{
	char msg[5];
	msg[0] = 's'; // wysyłam strzał
	msg[1] = s.x + 'A';
	msg[2] = s.y + 'A';
	msg[3] = '7'; //chyba nie będzie potrzebne, bo będziemy od razu wiedzieć którą wiadomość dostajemy
	msg[4] = '\0';
	sendStringToPipe(pipes, msg);
	//printf("Wysyłam %s %d %d %d %d\n", msg, msg[0], msg[1], msg[2], msg[3]);
}

void sendFeedback(PipesPtr pipes, Shoot s, Status stat)
{
	char msg[5];
	msg[0] = 'f'; // wysyłam wiadomość zwrotną
	msg[1] = s.x + 'A';
	msg[2] = s.y + 'A';
	msg[3] = stat;
	msg[4] = '\0'; //chyba nie będzie potrzebne,bo będziemy od razu wiedzieć którą wiadomość dostajemy
	
	sendStringToPipe(pipes, msg);
	//printf("Wysyłam %s\n", msg);
}

bool getMessage(PipesPtr pipes, char *msg)
{
	char msgg[5];
	if (getStringFromPipe(pipes, msgg, 5)) {
		strcpy(msg, msgg);
		//printf("przeczytałem: %s %d %d %d %d\n", msg, msg[0], msg[1], msg[2], msg[3]);
        msg[1] -= 'A';
        msg[2] -= 'A';
		return true;
	}
	return false;
}
