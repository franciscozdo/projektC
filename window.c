#include <gtk/gtk.h>
#include "fifo.h"
#include "msg.h"
#include "boards.h"
#include "game.h"

#define MAKS_DL_TEKSTU 3
#define N 10
#define S 2 // ile wolnych wierszy przed planszami

static GtkWidget *window;
static char *my_name, *opp_name;
static PipesPtr pipes;
static GtkWidget *my_but[100], *opp_but[100], *messages;
static Board my_board, opp_board;
static char /*mindex[100][4],*/ oindex[100][4];
static bool my_round;
static Ships my_ships, opp_ships;
static int longest_ship;

void pokazBlad(char *komunikat)
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

static void end (GtkWidget *widget, gpointer *data);
//static void send_move (GtkWidget *widget, GtkWidget *data);
static void getMove (GtkWidget *widget, char* ind);
static void changeButton (GtkWidget *widget, int status);
//static void incorrectShoot();
static gboolean refresh (gpointer data);
static void epilog (bool ifWin);

int main(int argc, char **argv) {

	if ((pipes = initPipes(argc,argv)) == NULL)
        return 1;
    if (argc == 2 && strcmp(argv[1],"A") == 0) {
		my_name = "A"; opp_name = "B";
        my_round = true;
	} else {
		my_name = "B"; opp_name = "A";
        my_round = false;
	}

	randBoard(my_board, my_ships, &longest_ship, my_name[0]);
    for (int i = 0; i < 10; ++i) opp_ships[i] = my_ships[i];
	clearBoard(opp_board);

	gtk_init(&argc, &argv);
	char win_name[] = "Statki x";
	sprintf(win_name, "Statki %s", my_name);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),win_name);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(end), NULL);

	
	GtkWidget *grid1 = gtk_grid_new();
	//gtk_widget_set_margin_end(box1, 5);
	//gtk_box_pack_start(GTK_BOX(box1), grid1, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), grid1);

    messages = gtk_label_new(NULL);
    gtk_label_set_text(GTK_LABEL(messages), "");
    
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	//gtk_container_add(GTK_CONTAINER(window), box1);
    gtk_grid_attach(GTK_GRID(grid1), box1, 0, 1, 21, 1);

    // To display messages from application

    messages = gtk_label_new(NULL);
    gtk_label_set_text(GTK_LABEL(messages), "");
	gtk_box_pack_start(GTK_BOX(box1), messages, FALSE, FALSE, 0);
    //gtk_grid_attach(GTK_GRID(grid1), messages, 5, 0, 11, 1);

    // Building boards

	for (int i = 0; i < N; ++i) {
		GtkWidget *rowname = gtk_label_new(NULL);
		char tekst[2];
		sprintf(tekst, "%d", i + 1);
		gtk_label_set_text(GTK_LABEL(rowname), (gchar*) tekst);
		gtk_grid_attach(GTK_GRID(grid1), rowname, i, S, 1, 1);
	}
	
	for (int i = 0; i < N; ++i) {
		GtkWidget *rowname = gtk_label_new(NULL);
		char tekst[2];
		sprintf(tekst, "%d", i + 1);
		gtk_label_set_text(GTK_LABEL(rowname), (gchar*) tekst);
		gtk_grid_attach(GTK_GRID(grid1), rowname, i + N + 1, S, 1, 1);
	}

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int ind = i * N + j;
			my_but[ind] = gtk_button_new();
            changeButton(my_but[ind], my_board[i][j]);
            //gtk_button_set_label(GTK_BUTTON(my_but[ind]), (my_board[i][j] == 0) ? "." : "X");
			//sprintf(mindex[ind], "m%d%d", i, j);
			//printf("%s ", mindex[ind]);
			//g_signal_connect(G_OBJECT(my_but[ind]), "clicked", G_CALLBACK(getMove), mindex[ind]);
			gtk_grid_attach(GTK_GRID(grid1), my_but[ind], i, j + 1 + S, 1, 1);
		}
	}
	for (int i = 0; i < N; ++i) {
		GtkWidget *rowname = gtk_label_new(NULL);
		char tekst[8];
		sprintf(tekst, "   %c   ", 'A' + i);
		gtk_label_set_text(GTK_LABEL(rowname), (gchar*) tekst);
		gtk_grid_attach(GTK_GRID(grid1), rowname, N, i + 1 + S, 1, 1);
	}

	//GtkWidget *separator = gtk_button_new();
	//gtk_grid_attach(GTK_GRID(grid1), separator, N, 0, 1, 10);
	
    for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int ind = i * N + j;
			opp_but[ind] = gtk_button_new();
			sprintf(oindex[ind], "%d%d", i, j);
	        changeButton(opp_but[ind], NOT_SHOOT);
            //gtk_button_set_label(GTK_BUTTON(opp_but[ind]), ".");
            //printf("%s ", oindex[ind]);
			g_signal_connect(G_OBJECT(opp_but[ind]), "clicked", G_CALLBACK(getMove), oindex[ind]);

			gtk_grid_attach(GTK_GRID(grid1), opp_but[ind], i + N + 1, j + 1 + S, 1, 1);
		}
	}

	//GtkWidget *text = gtk_entry_new();
    //gtk_entry_set_max_length(GTK_ENTRY(text), MAKS_DL_TEKSTU);
    //gtk_entry_set_text(GTK_ENTRY(text), "");
    //g_signal_connect(G_OBJECT(text), "activate",G_CALLBACK(send_move),(gpointer) text);
	//gtk_grid_attach(GTK_GRID(grid1), text, 5, N + 1, 11, 1);
	//gtk_box_pack_end(GTK_BOX(box1), text, FALSE, FALSE, 0);


    g_timeout_add(100,refresh,NULL);
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}

static void end (GtkWidget *widget, gpointer *data)
{
	closePipes(pipes);
	gtk_main_quit();
}

/*
static void send_move (GtkWidget *widget, GtkWidget *text)
{
	gchar in[MAKS_DL_TEKSTU];
	strcpy(in, gtk_entry_get_text( GTK_ENTRY(text)));
	gtk_entry_set_text( GTK_ENTRY(text), "");
	printf("%d%d%d %c%c%c\n", in[0], in[1], in[2], in[0], in[1], in[2]);
	/ *
	in[2] = 'a';
	int x = in[0] - 'A';
	int y = in[1] - '0' - 1; // -1 bo numerujemy od 0 a użytkownik wpisuje od 1
	if (isdigit(in[2]))		// <-----------------------To chyba nie działa bo nie czyta 10 :(
		y = y * 10 + in[2] - '0' - 1;
	* /
	Shoot s = makeShootFromStr(in);
	if (!isCorrect(s)) {
		pokazBlad("Niepoprawny starzal.\n");//incorrectShoot();
	} else{
		sendMove(pipes, s);	
		printf ("Wysłałem: %d %d\n", s.x, s.y);
	}
}
*/

static void getMove (GtkWidget *button, char* ind) {
	//printf("Naciśnięty %s\n", ind);
    if (!my_round) {
        gtk_label_set_text(GTK_LABEL(messages), "Nie Twoja runda.");
        return;
    }
    Shoot s = makeShoot(ind[0] - '0', ind[1] - '0');
    if (checkOnBoard(s, opp_board) == NOT_SHOOT){
        // Sending shoot and marking on board
        sendMove(pipes, s);
        markOnBoard(s, opp_board, UNKNOWN);
        int ind = s.x * N + s.y;
        changeButton(opp_but[ind], UNKNOWN);
        my_round = false;
        gtk_label_set_text(GTK_LABEL(messages), "");
        //printf("Wysłałem %d %d\n", s.x, s.y);
    } else {
        gtk_label_set_text(GTK_LABEL(messages), "W to pole już strzelałeś!");
        //pokazBlad("Jesteś DZBANEM!\n");
        //printf("już był strzelony\n");
    }
}

/*
static void incorrectShoot()
{
    GtkWidget *dialog;
	char komunikat[100] = "Niepoprawne współrzędne strzału.\n Spróbuj jeszcze raz.\n";
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);

}
*/

static void changeButton(GtkWidget *button, int stat) 
{
    //printf("%d ", stat);
    //if(stat == NOT_SHOOT) return;
    char t[2];
    if (stat == HIT)
        sprintf(t, "T");
    else if (stat == SUNK)
        sprintf(t, "Z");
    else if (stat == MISSED)
        sprintf(t, "P");
    else if (stat == UNKNOWN)
        sprintf(t, "U");
    else if (stat == MY_HIT)
        sprintf(t, "T");
    else if (stat == SHIP)
        sprintf(t, "X");
    else if (stat == NOT_SHOOT)
        sprintf(t, ".");

    gtk_button_set_label(GTK_BUTTON(button), t);
}

static void updateBoard(char c)
{
    if (c == 'm') {
        for (int i = 0; i < 100; ++i) {
            changeButton (my_but[i], my_board[i/10][i%10]);
        }
    } else {
        for (int i = 0; i < 100; ++i) {
            changeButton (opp_but[i], opp_board[i/10][i%10]);
        }
    }
}

static gboolean refresh(gpointer data)
{
	char msg[5];
	if (getMessage(pipes, msg)) {
		//printf("Dostałem wiadomość: %d %d\n", msg[1], msg[2]);
		if (msg[0] == 's') {
            // Sending feedback and marking shoot on board
            //printf("%d\n", ind);
            
            Shoot s = makeShoot(msg[1], msg[2]);
            int ind = s.x * N + s.y;
            Status stat = checkOnBoard(s, my_board) == SHIP ? HIT : MISSED;
            //stat = (stat > 0 && stat <= 5) ? HIT : MISSED;
            markOnBoard(s, my_board, stat == HIT ? MY_HIT : MISSED);
            
            bool vis[10][10];
            int size_of_ship = 0;
            for (int i = 0; i < 100; ++i) vis[i/10][i%10] = false;
            
            if (stat == HIT && isSunk(s, my_board, vis)) {
                size_of_ship = markSunk(s, my_board);
                //printf("%s:%d\n",my_name,size_of_ship);
                stat = SUNK;
                updateBoard('m');
                --my_ships[size_of_ship];
                
            } else {
                changeButton(my_but[ind], stat);
            }
            //printf("%sw%d\n",my_name, stat);
            sendFeedback(pipes, s, stat);
            if (my_ships[size_of_ship] == 0) {
                if (allSunk(my_ships, longest_ship)) {
                    epilog(false); // you loose
                    //end();
                }
            }
            my_round = true;
            gtk_label_set_text(GTK_LABEL(messages), "Twoja kolej!");
		} else if (msg[0] == 'f') {
			// Reading feedback and marking it on board etc.
            //printf("Dostałem %c %d %d %d\n", msg[0], msg[1], msg[2], msg[3]);

            Shoot s = makeShoot(msg[1], msg[2]);
            int ind = s.x * N + s.y;
            Status stat = msg[3];
            //stat = (stat > 0 && stat <= 5) ? HIT : MISSED;
            //printf("%sd%d\n", my_name, stat);
            int size_of_ship = 0;
            
            if (stat == SUNK) {
                markOnBoard(s, opp_board, HIT);
                size_of_ship = markSunk(s, opp_board);
                //printf("%s: %d\n", my_name, size_of_ship);
                stat = SUNK;
                updateBoard('o');
                --opp_ships[size_of_ship];
                if (opp_ships[size_of_ship] == 0) {
                    if (allSunk(opp_ships, longest_ship)) {
                        epilog(true);
                        //end();
                    }
                }   
            } else {
                markOnBoard(s, opp_board, stat);
                changeButton(opp_but[ind], stat);
            }
            /*Shoot s = makeShoot(msg[1], msg[2]);
            Status stat = msg[3];
            changeButton(opp_but[ind], stat);
            markOnBoard(s, opp_board, stat);*/
		}
	}
	return TRUE;
}

static void epilog (bool win)
{
    char alert[100];
    if (win) {
        sprintf(alert, "Wygrałeś.\n Zestrzeliłeś wszystkie statki gracza %s\n", opp_name);
        pokazBlad(alert);
    } else {
        sprintf(alert, "Przegrałeś.\n Gracz %s zestrzelił Ci wszystkie statki.\n", opp_name);
        pokazBlad(alert);
    }
    end(NULL, NULL);
}
