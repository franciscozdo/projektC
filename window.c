#include <gtk/gtk.h>
#include "fifo.h"
#include "msg.h"

#define MAKS_DL_TEKSTU 3

static GtkWidget *window, *buffer;
static char *my_name, *opponent_name;
static PipesPtr pipes;

/*
 void refresh(GtkWidget *widget, gpointer data) {
	g_print("Ten przycisk będzie kiedyś służył do odświeżania\n");
}
*/

void pokazBlad(char *komunikat)
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

static void end (GtkWidget *widget, gpointer *data);
static void send_move (GtkWidget *widger, GtkWidget *data);
static gboolean refresh(gpointer data);

int main(int argc, char **argv) {

	if ((pipes = initPipes(argc,argv)) == NULL)
        return 1;
    if (argc == 2 && strcmp(argv[1],"A") == 0) {
		my_name = "A"; opponent_name = "B";
	} else {
		my_name = "B"; opponent_name = "A";
	}

	gtk_init(&argc, &argv);
	char win_name[] = "Statki x";
	sprintf(win_name, "Statki %s", my_name);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),win_name);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(end), NULL);

	GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(window), box1);

	buffer = (GtkWidget *)gtk_text_buffer_new (NULL);
    GtkWidget *text_view = gtk_text_view_new_with_buffer (GTK_TEXT_BUFFER(buffer));
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);

	//gtk_grid_attach(GTK_GRID(grid), text_view, 0, 0, 60, 20);
	gtk_box_pack_end(GTK_BOX(box1), text_view, FALSE, FALSE, 0);

	GtkWidget *text = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(text), MAKS_DL_TEKSTU);
    gtk_entry_set_text(GTK_ENTRY(text), "");
    g_signal_connect(G_OBJECT(text), "activate",G_CALLBACK(send_move),(gpointer) text);
    //gtk_grid_attach(GTK_GRID(grid), text, 0, 20, 60, 1);
	gtk_box_pack_end(GTK_BOX(box1), text, FALSE, FALSE, 0);
	/*
	GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, FALSE, 0);
	*/
	/*
	GtkWidget *but_refresh = gtk_button_new();//_with_label("Refresh");
	GtkWidget *imag = gtk_image_new();
	gtk_image_set_from_icon_name(GTK_IMAGE(imag), "view-refresh", GTK_ICON_SIZE_LARGE_TOOLBAR);
	gtk_button_set_always_show_image(GTK_BUTTON(but_refresh), TRUE);
	gtk_button_set_image(GTK_BUTTON(but_refresh), imag);

	g_signal_connect(G_OBJECT(but_refresh), "clicked", G_CALLBACK(refresh), NULL);
	gtk_box_pack_end(GTK_BOX(box2), but_refresh, FALSE, FALSE, 0);
	*/

	/*
	GtkWidget *box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(box1), box3, FALSE, FALSE, 0);
	*/

	// Tu trzeba jakieś cholerstwo do wyświetlania plansz wstawić

    //g_timeout_add(100,refresh,NULL);
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}

static void end (GtkWidget *widget, gpointer *data)
{
	closePipes(pipes);
	gtk_main_quit();
}

static void send_move (GtkWidget *widger, GtkWidget *text)
{
	gchar in[MAKS_DL_TEKSTU];
	strcpy(in, gtk_entry_get_text( GTK_ENTRY(text)));
	gtk_entry_set_text( GTK_ENTRY(text), "");
	int x = in[0] - 'A';
	int y = in[1] - '0' - 1; // -1 bo numerujemy od 0 a użytkownik wpisuje od 1
	if (isdigit(in[2]))		// <------------------------------------------------To chyba nie działa bo nie czyta 10 :(
		y = y * 10 + in[2] - '0' - 1;
	Shoot s = makeShoot(x, y);
	sendMove(pipes, s);	
	printf ("Wysłałem: %d %d\n", x, y);
}
/*
static gboolean refresh(gpointer data)
{
	/*
	 * Sprawdzenie wiadomosci
	 * zaktualizowanie statusu
	 * wysłanie jeśli trzeba
	 * /	
	return 1;
}
*/
