#include <gtk/gtk.h>
#include "fifo.h"
#include "msg.h"
#include "game.h"

#define MAKS_DL_TEKSTU 3
#define N 10

static GtkWidget *window;
static char *my_name, *opponent_name;
static PipesPtr pipes;
static GtkWidget *my_but[100], *opp_but[100];
static Board my_board, opp_board;

void pokazBlad(char *komunikat)
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

static void end (GtkWidget *widget, gpointer *data);
static void send_move (GtkWidget *widget, GtkWidget *data);
static void get_move (GtkWidget * widget, char* ind);
static void incorrectShoot();
static gboolean refresh(gpointer data);

int main(int argc, char **argv) {

	if ((pipes = initPipes(argc,argv)) == NULL)
        return 1;
    if (argc == 2 && strcmp(argv[1],"A") == 0) {
		my_name = "A"; opponent_name = "B";
	} else {
		my_name = "B"; opponent_name = "A";
	}

	clearBoard(my_board);
	clearBoard(opp_board);

	gtk_init(&argc, &argv);
	char win_name[] = "Statki x";
	sprintf(win_name, "Statki %s", my_name);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),win_name);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(end), NULL);

	GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(window), box1);

	GtkWidget *grid1 = gtk_grid_new();
	gtk_box_pack_start(GTK_BOX(box1), grid1, FALSE, FALSE, 0);
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int ind = i * N + j;
			my_but[ind] = gtk_button_new();
			char index[3];
			sprintf(index, "%d%d", i, j);
			g_signal_connect(G_OBJECT(my_but[ind]), "clicked", G_CALLBACK(get_move), index);
			gtk_grid_attach(GTK_GRID(grid1), my_but[ind], i, j, 1, 1);
		}
	}
	GtkWidget *separator = gtk_button_new();
	gtk_grid_attach(GTK_GRID(grid1), separator, N, 0, 1, 10);

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int ind = i * N + j;
			opp_but[ind] = gtk_button_new();
			gtk_grid_attach(GTK_GRID(grid1), opp_but[ind], i + N + 1, j, 1, 1);
		}
	}

	GtkWidget *text = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(text), MAKS_DL_TEKSTU);
    gtk_entry_set_text(GTK_ENTRY(text), "");
    g_signal_connect(G_OBJECT(text), "activate",G_CALLBACK(send_move),(gpointer) text);
	gtk_box_pack_end(GTK_BOX(box1), text, FALSE, FALSE, 0);


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

static void send_move (GtkWidget *widget, GtkWidget *text)
{
	gchar in[MAKS_DL_TEKSTU];
	strcpy(in, gtk_entry_get_text( GTK_ENTRY(text)));
	gtk_entry_set_text( GTK_ENTRY(text), "");
	printf("%d%d%d %c%c%c\n", in[0], in[1], in[2], in[0], in[1], in[2]);
	/*
	in[2] = 'a';
	int x = in[0] - 'A';
	int y = in[1] - '0' - 1; // -1 bo numerujemy od 0 a użytkownik wpisuje od 1
	if (isdigit(in[2]))		// <-----------------------To chyba nie działa bo nie czyta 10 :(
		y = y * 10 + in[2] - '0' - 1;
	*/
	Shoot s = makeShootFromStr(in);
	if (!isCorrect(s)) {
		incorrectShoot();
	} else{
		sendMove(pipes, s);	
		printf ("Wysłałem: %d %d\n", s.x, s.y);
	}
}

static void get_move (GtkWidget *button, char* ind) {
	printf("Naciśnięty %s\n", ind);
}

static void incorrectShoot()
{
    GtkWidget *dialog;
	char komunikat[100] = "Niepoprawne współrzędne strzału.\n Spróbuj jeszcze raz.\n";
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);

}

static gboolean refresh(gpointer data)
{
	char msg[5];
	if (getMessage(pipes, msg)) {
		printf("Dostałem wiadomość: %d %d\n", msg[1], msg[2]);
		if (msg[0] == 's') {
			// zaznaczanie strzału i wysyłanie feedbacku
		} else if (msg[0] == 'f') {
			// aktualizacja
		}
	}
	return TRUE;
}

