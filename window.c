#include <gtk/gtk.h>
#include "fifo.h"
#include "msg.h"
#include "boards.h"
#include "game.h"

#define MAKS_DL_TEKSTU 3
#define N 10
#define S 2 // ile wolnych wierszy przed planszami

static GtkWidget *window, *sec_win;
static char *my_name, *opp_name;
static PipesPtr pipes;
static GtkWidget *my_but[100], *opp_but[100], *creat_but[100];
static GtkWidget *my_but_im[100], *opp_but_im[100], *creat_but_im[100];
static GtkWidget *messages, *sec_messages;
static struct panel {
    GtkWidget *but[5];
    char index[5][4];
    bool waiting;
    int length;
} ship_len;
static struct panel2 {
    GtkWidget *vert_but, *horizon_but;
    bool waiting;
    int orientation;
} orient;
static Board my_board, opp_board, creat_board;
static char oindex[100][4], cindex[100][4];
static bool my_round, game_run = false; // game_run - if game is started it's true
static Ships my_ships, opp_ships, creat_ships;


void pokazBlad(char *komunikat);
static void end (GtkWidget *widget, gpointer *data);
static void getMove (GtkWidget *widget, char* ind);
static void new_game (GtkWidget *widget, gpointer *data);
static void give_up (GtkWidget *widget, gpointer *data);
static void rand_board (GtkWidget *widget, gpointer *data);
static void create_board (GtkWidget *widget, gpointer *data);
static void quit_sec (GtkWidget *widget, gpointer *data) ;
static void confirm (GtkWidget *widget, gpointer *data);
static void creatShip (GtkWidget *widget, char* ind);
static void change_orient(GtkWidget *widget, char *ind);
static void change_len(GtkWidget *widget, char *ind);
static gboolean refresh (gpointer data);
static void changeButton (GtkWidget *widget, int status);
static void epilog (char option);


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

	gtk_init(&argc, &argv);

    //water = gtk_image_new_from_file("data/water.png");
    //ship = gtk_image_new_from_file("data/ship.png");
    //cloud = gtk_image_new_from_file("data/cloud.png");
    //sunk = gtk_image_new_from_file("data/sunk.png");

	char win_name[12];
	sprintf(win_name, "Statki %s", my_name);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),win_name);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(end), NULL);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // LABEL TO DISPLAY MESSAGES

    messages = gtk_label_new("");
    gtk_label_set_text(GTK_LABEL(messages), "");
	gtk_box_pack_start(GTK_BOX(main_box), messages, FALSE, FALSE, 5);
    //gtk_box_pack_start(GTK_BOX(main_box), graphic.water, FALSE, FALSE, 5);
	
    GtkWidget *main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(main_box), main_hbox, TRUE, TRUE, 5);

    // MAIN GRID

	GtkWidget *main_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(main_grid), 0);
    gtk_grid_set_column_spacing(GTK_GRID(main_grid), 0);
    //gtk_grid_set_row_homogeneous(GTK_GRID(main_grid), TRUE);
    //gtk_grid_set_column_homogeneous(GTK_GRID(main_grid), TRUE);
	gtk_box_pack_start(GTK_BOX(main_hbox), main_grid, FALSE, FALSE, 0);

    
    // BUILDING BOARDS WITH LABELS

	for (int i = 0; i < N; ++i) {
		GtkWidget *rowname = gtk_label_new(NULL);
		char tekst[2];
		sprintf(tekst, "%d", i + 1);
		gtk_label_set_text(GTK_LABEL(rowname), (gchar*) tekst);
		gtk_grid_attach(GTK_GRID(main_grid), rowname, i, S, 1, 1);
	}
	
	for (int i = 0; i < N; ++i) {
		GtkWidget *rowname = gtk_label_new(NULL);
		char tekst[2];
		sprintf(tekst, "%d", i + 1);
		gtk_label_set_text(GTK_LABEL(rowname), (gchar*) tekst);
		gtk_grid_attach(GTK_GRID(main_grid), rowname, i + N + 1, S, 1, 1);
	}

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int ind = i * N + j;
			my_but[ind] = gtk_button_new();
            gtk_button_set_image(GTK_BUTTON(my_but[ind]), my_but_im[ind]);
            //changeButton(my_but[ind], my_board[i][j]);
            //gtk_button_set_label(GTK_BUTTON(my_but[ind]), (my_board[i][j] == 0) ? "." : "X");
			//sprintf(mindex[ind], "m%d%d", i, j);
			//printf("%s ", mindex[ind]);
			//g_signal_connect(G_OBJECT(my_but[ind]), "clicked", G_CALLBACK(getMove), mindex[ind]);
			gtk_grid_attach(GTK_GRID(main_grid), my_but[ind], i, j + 1 + S, 1, 1);
		}
	}
	for (int i = 0; i < N; ++i) {
		GtkWidget *rowname = gtk_label_new(NULL);
		char tekst[8];
		sprintf(tekst, "   %c   ", 'A' + i);
		gtk_label_set_text(GTK_LABEL(rowname), (gchar*) tekst);
		gtk_grid_attach(GTK_GRID(main_grid), rowname, N, i + 1 + S, 1, 1);
	}

	//GtkWidget *separator = gtk_button_new();
	//gtk_grid_attach(GTK_GRID(main_grid), separator, N, 0, 1, 10);
	
    for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int ind = i * N + j;
			opp_but[ind] = gtk_button_new();
			sprintf(oindex[ind], "%d%d", i, j);
            //im = gtk_image_new_from_file("data/water.png");
            gtk_button_set_image(GTK_BUTTON(opp_but[ind]), opp_but_im[ind]);
	        //changeButton(opp_but[ind], NOT_SHOOT);
            //gtk_button_set_label(GTK_BUTTON(opp_but[ind]), ".");
            //printf("%s ", oindex[ind]);
			g_signal_connect(G_OBJECT(opp_but[ind]), "clicked", G_CALLBACK(getMove), oindex[ind]);

			gtk_grid_attach(GTK_GRID(main_grid), opp_but[ind], i + N + 1, j + 1 + S, 1, 1);
		}
	}
    
    GtkWidget *left_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(left_panel), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(left_panel), 0);
    gtk_box_pack_start(GTK_BOX(main_hbox), left_panel, TRUE, FALSE, 0);
    //gtk_grid_attach(GTK_GRID(main_grid), left_panel, 2 * N + 2, S, 1, 11);

    // BUTTONS TO CONTROL THE GAME (NEW GEME, ETC.)
    
    GtkWidget *ctrl_frame = gtk_frame_new("Controls");
    //gtk_container_set_border_width(GTK_CONTAINER(ctrl_frame), 10);
    gtk_box_pack_start(GTK_BOX(left_panel), ctrl_frame, FALSE, FALSE, 0);
    //gtk_grid_attach(GTK_GRID(main_grid), ctrl_frame, 2 * N + 2, S + 3, 1, 5);
    
    GtkWidget *controls = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(controls), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(controls), 5);
	gtk_container_add(GTK_CONTAINER(ctrl_frame), controls);

    GtkWidget *new_game_but = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(new_game_but), "Nowa Gra");
    gtk_box_pack_start(GTK_BOX(controls), new_game_but, FALSE, FALSE, 0);
    //gtk_grid_attach(GTK_GRID(main_grid), new_game_but, 2 * N + 2, S + 1, 1, 1);
    g_signal_connect(G_OBJECT(new_game_but), "clicked", G_CALLBACK(new_game), "n");
    
    GtkWidget *rand_board_but = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(rand_board_but), "Losowa Plansza");
    gtk_box_pack_start(GTK_BOX(controls), rand_board_but, FALSE, FALSE, 0);
    //gtk_grid_attach(GTK_GRID(main_grid), rand_board_but, 2 * N + 2, S + 2, 1, 1);
    g_signal_connect(G_OBJECT(rand_board_but), "clicked", G_CALLBACK(rand_board), NULL);
    
    GtkWidget *load_but = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(load_but), "Stwórz Planszę");
    gtk_box_pack_start(GTK_BOX(controls), load_but, FALSE, FALSE, 0);
    //gtk_grid_attach(GTK_GRID(main_grid), load_but, 2 * N + 2, S + 3, 1, 1);
    g_signal_connect(G_OBJECT(load_but), "clicked", G_CALLBACK(create_board), NULL);
    
    GtkWidget *give_up_but = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(give_up_but), "Poddaj Się");
    gtk_box_pack_start(GTK_BOX(controls), give_up_but, FALSE, FALSE, 0);
    //gtk_grid_attach(GTK_GRID(main_grid), give_up_but, 2 * N + 2, S + 4, 1, 1);
    g_signal_connect(G_OBJECT(give_up_but), "clicked", G_CALLBACK(give_up), NULL);
    
    GtkWidget *quit_but = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(quit_but), "Zakończ");
    gtk_box_pack_start(GTK_BOX(controls), quit_but, FALSE, FALSE, 0);
    //gtk_grid_attach(GTK_GRID(main_grid), quit_but, 2 * N + 2, S + 4, 1, 1);
    g_signal_connect(G_OBJECT(quit_but), "clicked", G_CALLBACK(end), NULL);
    
    new_game(NULL, NULL);
    

    // STATISTICS
    /*
    GtkWidget *stat_frame = gtk_frame_new("Statistics");
    //gtk_container_set_border_width(GTK_CONTAINER(stat_frame), 5);
    gtk_box_pack_start(GTK_BOX(left_panel), stat_frame, FALSE, FALSE, 0);
    //gtk_grid_attach(GTK_GRID(main_grid), stat_frame, 2 * N + 2, S + 5, 1, 5);

    GtkWidget *statistics = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(controls), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(controls), 5);
	gtk_container_add(GTK_CONTAINER(stat_frame), statistics);

    GtkWidget *op_sunk = gtk_label_new("Zatopione statki: 0");
    gtk_box_pack_start(GTK_BOX(statistics), op_sunk, FALSE, FALSE, 0);
    */

	
    g_timeout_add(100,refresh,NULL);
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}

void pokazBlad(char *komunikat)
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

static void updateBoard(char c)
{
            //puts("xd");
    if (c == 'm') {
        for (int i = 0; i < 100; ++i) {
            changeButton (my_but[i], my_board[i/10][i%10]);
        }
    } else if (c == 'o'){
        for (int i = 0; i < 100; ++i) {
            //printf("%d\n", i);
            changeButton (opp_but[i], opp_board[i/10][i%10] == NOT_SHOOT ? UNKNOWN : opp_board[i/10][i%10]);
        }

    } else if (c == 'c') {
         for (int i = 0; i < 100; ++i) {
            changeButton (creat_but[i], creat_board[i/10][i%10]);
        }
    }
}

static void create_board (GtkWidget *widget, gpointer *data)
{
    if (game_run) return;

    sec_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(sec_win), "Wybieranie planszy");
	gtk_container_set_border_width(GTK_CONTAINER(sec_win), 10);
	g_signal_connect(G_OBJECT(sec_win), "destroy", G_CALLBACK(quit_sec), NULL);

    GtkWidget *sec_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(sec_grid), 0);
    gtk_grid_set_column_spacing(GTK_GRID(sec_grid), 0);
	gtk_container_add(GTK_CONTAINER(sec_win), sec_grid);
    
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_grid_attach(GTK_GRID(sec_grid), box1, 0, 1, 10, 1);

    // LABEL TO DISPLAY MESSAGES

    sec_messages = gtk_label_new(NULL);
    gtk_label_set_text(GTK_LABEL(sec_messages), "Wybierz pole na który ma się zaczynać statek.");
	gtk_box_pack_end(GTK_BOX(box1), sec_messages, TRUE, FALSE, 5);

    GtkWidget *add_ship = gtk_frame_new("Nowy statek");
    gtk_grid_attach(GTK_GRID(sec_grid), add_ship, 0, 2, 11, 1);
   
    GtkWidget *box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(box3), 5);
    gtk_container_add(GTK_CONTAINER(add_ship), box3);
    // BUTTONS TO SET SHIP TO INSERT

    /*GtkWidget *orient_lab = gtk_label_new("Orientacja:");
    gtk_box_pack_start(GTK_BOX(box3), orient_lab, FALSE, FALSE, 5);
    */
    
    orient.waiting = true;
    ship_len.waiting = true;

    orient.horizon_but = gtk_toggle_button_new();
    gtk_button_set_label(GTK_BUTTON(orient.horizon_but), "Poziomy");
    gtk_box_pack_start(GTK_BOX(box3), orient.horizon_but, FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(orient.horizon_but), "toggled", G_CALLBACK(change_orient), "h");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(orient.horizon_but), TRUE);
    
    orient.vert_but = gtk_toggle_button_new();
    gtk_button_set_label(GTK_BUTTON(orient.vert_but), "Pionowy");
    gtk_box_pack_start(GTK_BOX(box3), orient.vert_but, FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(orient.vert_but), "toggled", G_CALLBACK(change_orient), "v");
    
    GtkWidget *length_lab = gtk_label_new(" Długość: ");
    gtk_box_pack_start(GTK_BOX(box3), length_lab, FALSE, FALSE, 0);
    
    
    for (int i = 0; i < N; ++i) {
		GtkWidget *rowname = gtk_label_new(NULL);
		char tekst[2];
		sprintf(tekst, "%d", i + 1);
		gtk_label_set_text(GTK_LABEL(rowname), (gchar*) tekst);
		gtk_grid_attach(GTK_GRID(sec_grid), rowname, i + 1, 3, 1, 1);
	}
    
    for (int i = 0; i < 5; ++i) {
        ship_len.but[i] = gtk_toggle_button_new();
        sprintf(ship_len.index[i], "%d", i + 1);
        gtk_button_set_label(GTK_BUTTON(ship_len.but[i]), ship_len.index[i]);
        gtk_box_pack_start(GTK_BOX(box3), ship_len.but[i], FALSE, FALSE, 0);
        g_signal_connect(G_OBJECT(ship_len.but[i]), "toggled", G_CALLBACK(change_len), ship_len.index[i]);
    }
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ship_len.but[4]), TRUE);
    
    for (int i = 0; i < N; ++i) {
		GtkWidget *rowname = gtk_label_new(NULL);
		char tekst[8];
		sprintf(tekst, "   %c   ", 'A' + i);
		gtk_label_set_text(GTK_LABEL(rowname), (gchar*) tekst);
		gtk_grid_attach(GTK_GRID(sec_grid), rowname, 0, i + 4, 1, 1);
	}
    
    /*for (int i = 0; i < N; ++i) {
		GtkWidget *rowname = gtk_label_new(NULL);
		char tekst[8];
		sprintf(tekst, "   %c   ", 'A' + i);
		gtk_label_set_text(GTK_LABEL(rowname), (gchar*) tekst);
		gtk_grid_attach(GTK_GRID(sec_grid), rowname, N + 1, i + 4, 1, 1);
	}*/
    
    for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int ind = i * N + j;
			creat_but[ind] = gtk_button_new();
			sprintf(cindex[ind], "%d%d", i, j);
            gtk_button_set_image(GTK_BUTTON(creat_but[ind]), creat_but_im[ind]);
	        changeButton(creat_but[ind], NOT_SHOOT);
			g_signal_connect(G_OBJECT(creat_but[ind]), "clicked", G_CALLBACK(creatShip), cindex[ind]);

			gtk_grid_attach(GTK_GRID(sec_grid), creat_but[ind], i + 1, j + 4, 1, 1);
		}
	}
    

    GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(box2), 10);
    gtk_grid_attach(GTK_GRID(sec_grid), box2, 0, N + 4, 10, 1);

    GtkWidget *cancel_but = gtk_button_new(); 
    gtk_button_set_label(GTK_BUTTON(cancel_but), "Anuluj");
    g_signal_connect(G_OBJECT(cancel_but), "clicked", G_CALLBACK(quit_sec), NULL);
    gtk_box_pack_end(GTK_BOX(box2), cancel_but, FALSE, FALSE, 5);
    
    GtkWidget *confirm_but = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(confirm_but), "Zatwierdź");
    gtk_box_pack_end(GTK_BOX(box2), confirm_but, FALSE, FALSE, 5);
    g_signal_connect(G_OBJECT(confirm_but), "clicked", G_CALLBACK(confirm), NULL);


    clearBoard(creat_board);

    gtk_widget_show_all(sec_win);
}

static void change_orient (GtkWidget *button, char *ind) 
{
    if (orient.waiting == false) return;
    orient.waiting = false;
    //printf("toggled %s\n", ind);
    gtk_label_set_text(GTK_LABEL(sec_messages), "");

    if (ind[0] == 'v') {
        orient.orientation = 0;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(orient.horizon_but), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(orient.vert_but), TRUE);
    } else {
        orient.orientation = 1;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(orient.horizon_but), TRUE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(orient.vert_but), FALSE);
    }
    orient.waiting = true;
}

static void change_len (GtkWidget *button, char *ind)
{
    if (ship_len.waiting == false) return;
    ship_len.waiting = false;
    gtk_label_set_text(GTK_LABEL(sec_messages), "");

    int id = ind[0] - '0' - 1;
    for (int i = 0; i < 5; ++i) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ship_len.but[i]), FALSE);
    }
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ship_len.but[id]), TRUE);
    ship_len.length = id + 1;

    ship_len.waiting = true;
}

static void creatShip (GtkWidget *button, char* ind)
{
    //printf("orient: %d length: %d\n", orient.orientation, ship_len.length);
    Shoot pos = makeShoot(ind[0] - '0', ind[1] - '0');
    gtk_label_set_text(GTK_LABEL(sec_messages), "");
    
    if (checkOnBoard(pos, creat_board) == SHIP) {
        int len = removeShip(pos, creat_board);
        ++creat_ships.count[len];
        //printf("usun %d\n", len);
    } else {
        if (creat_ships.count[ship_len.length] == 0) {
            gtk_label_set_text(GTK_LABEL(sec_messages),"Statków tej długości jest już wystarczająco dużo.");
            return;
        }
        int id = pos.x + pos.y * 10;//ind[0] - '0' + (ind[1] - '0') * 10;
        if (placeShip(creat_board, ship_len.length, id, orient.orientation)) {
            --creat_ships.count[ship_len.length];
        } else {
            gtk_label_set_text(GTK_LABEL(sec_messages),"Tutaj nie możesz postawić statku.");
            return;
        } 
    }
    updateBoard('c');
}

static void confirm (GtkWidget *widget, gpointer *data)
{
    if (allSunk(creat_ships)) {
        copyBoard(my_board, creat_board);
        updateBoard('m');
        quit_sec(NULL, NULL);
    } else {
        gtk_label_set_text(GTK_LABEL(sec_messages), "Ustaw wszystkie statki");
    }   
}

static void quit_sec (GtkWidget *widget, gpointer *data) 
{
    gtk_widget_destroy(sec_win);
}

static void end (GtkWidget *widget, gpointer *data)
{
	closePipes(pipes);
	gtk_main_quit();
}

static void getMove (GtkWidget *button, char* ind) {
    if (!my_round) {
        gtk_label_set_text(GTK_LABEL(messages), "Nie Twoja runda.");
        return;
    }
    Shoot s = makeShoot(ind[0] - '0', ind[1] - '0');
    if (checkOnBoard(s, opp_board) == NOT_SHOOT){
        game_run = true;
        // Sending shoot and marking on board
        markOnBoard(s, opp_board, UNKNOWN);
        int ind = s.x * N + s.y;
        changeButton(opp_but[ind], UNKNOWN);
        my_round = false;
        gtk_label_set_text(GTK_LABEL(messages), "");
        sendMove(pipes, s);
    } else {
        gtk_label_set_text(GTK_LABEL(messages), "W to pole już strzelałeś!");
    }
}

static void changeButton(GtkWidget *button, int stat) 
{
    GtkWidget *im;
    gtk_widget_set_size_request(button, 35, 35);
    if (stat == HIT || stat == SHIP){
        im = gtk_image_new_from_file("data/ship.png");
        gtk_button_set_image(GTK_BUTTON(button), im);//sprintf(t, "T");
    }else if (stat == SUNK || stat == MY_HIT){
        im = gtk_image_new_from_file("data/sunk.png");
        gtk_button_set_image(GTK_BUTTON(button), im);//sprintf(t, "Z");
    }else if (stat == MISSED || stat == NOT_SHOOT){
        im = gtk_image_new_from_file("data/water.png");
        gtk_button_set_image(GTK_BUTTON(button), im);//sprintf(t, "P");
    }else if (stat == UNKNOWN){
        im = gtk_image_new_from_file("data/cloud.png");
        gtk_button_set_image(GTK_BUTTON(button), im);//sprintf(t, "U");
    }
//    else if (stat == MY_HIT)
//        gtk_button_set_image(GTK_BUTTON(button), sunk);//sprintf(t, "T");
//    else if (stat == SHIP)
//        gtk_button_set_image(GTK_BUTTON(button), ship);//sprintf(t, "X");
//    else if (stat == NOT_SHOOT)
//        gtk_button_set_image(GTK_BUTTON(button), water);//sprintf(t, ".");

    //gtk_button_set_label(GTK_BUTTON(button), t);
}



static gboolean refresh(gpointer data)
{
	char msg[5];
	if (getMessage(pipes, msg)) {
		//printf("Dostałem wiadomość: %d %d\n", msg[1], msg[2]);
		if (msg[0] == 's') {
            // Sending feedback and marking shoot on board
            game_run = true;
            Shoot s = makeShoot(msg[1], msg[2]);
            int ind = s.x * N + s.y;
            Status stat = checkOnBoard(s, my_board) == SHIP ? HIT : MISSED;
            //stat = (stat > 0 && stat <= 5) ? HIT : MISSED;
            markOnBoard(s, my_board, stat == HIT ? MY_HIT : MISSED);
            
            bool vis[10][10];
            int size_of_ship = 0;
            for (int i = 0; i < 144; ++i) vis[i/10][i%10] = false;
            if (stat == HIT && isSunk(s, my_board, vis)) {
                size_of_ship = markSunk(s, my_board);
                //printf("%s:%d\n",my_name,size_of_ship);
                stat = SUNK;
                updateBoard('m');
                --my_ships.count[size_of_ship];
                
            } else {
                changeButton(my_but[ind], stat);
            }
            //printf("%sw%d\n",my_name, stat);
            sendFeedback(pipes, s, stat);
            if (my_ships.count[size_of_ship] == 0) {
                if (allSunk(my_ships)) {
                    epilog('l'); // you loose
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
                //stat = SUNK;
                updateBoard('o');
                --opp_ships.count[size_of_ship];
                //printf("%d %d<-\n", size_of_ship, opp_ships.count[size_of_ship]);
                if (opp_ships.count[size_of_ship] == 0) {
                    if (allSunk(opp_ships)) {
                        epilog('w');
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
		} else if (msg[0] == 'g') {
            epilog('g');
        } else if (msg[0] == 'n') {
            game_run = false;
            clearBoard(opp_board);
	        randBoard(my_board, &my_ships, my_name[0]);
            updateBoard('m');
            updateBoard('o');
        }
	}
	return TRUE;
}

static void epilog (char option)
{
    char alert[100];
    if (option == 'w') {
        sprintf(alert, "Wygrałeś.\n Zestrzeliłeś wszystkie statki gracza %s\n", opp_name);
        pokazBlad(alert);
        new_game(NULL, (gpointer)"N");
    } else if (option == 'l'){
        sprintf(alert, "Przegrałeś.\n Gracz %s zestrzelił Ci wszystkie statki.\n", opp_name);
        pokazBlad(alert);
        new_game(NULL, NULL);
    } else if (option == 'g') {
        sprintf(alert, "Twój przeciwnik poddał się.\nWygrałeś.\n");
        pokazBlad(alert);
    }
    //end(NULL, NULL);
}

static void new_game (GtkWidget *widget, gpointer *data)
{
    game_run = false;
    if(data != NULL) sendSignal(pipes, 1);
    clearBoard(opp_board);
	randBoard(my_board, &my_ships, my_name[0]);
    for (int i = 0; i < 10; ++i) opp_ships.count[i] = my_ships.count[i], creat_ships.count[i] = my_ships.count[i];
    opp_ships.longest = my_ships.longest;
    updateBoard('m');
    updateBoard('o');
}

static void give_up (GtkWidget *widget, gpointer *data)
{
    sendSignal(pipes, 2);
    new_game(NULL, (gpointer)"N");
}

static void rand_board (GtkWidget *widget, gpointer *data)
{
    if (!game_run) {
	    randBoard(my_board, &my_ships, my_name[0]);
        updateBoard('m');
    } else {
        gtk_label_set_text(GTK_LABEL(messages), "Nie możesz już zmienić planszy. Gra się rozpoczęła.");
    }
}
