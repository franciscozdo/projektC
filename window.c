#include <gtk/gtk.h>

void refresh(GtkWidget *widget, gpointer data) {
	g_print("Ten przycisk będzie kiedyś służył do odświeżania\n");
}

int main(int argc, char **argv) {

	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Statki");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(window), box1);

	GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, FALSE, 0);

	GtkWidget *but_refresh = gtk_button_new();//_with_label("Refresh");
	GtkWidget *imag = gtk_image_new();
	gtk_image_set_from_icon_name(GTK_IMAGE(imag), "view-refresh", GTK_ICON_SIZE_LARGE_TOOLBAR);
	gtk_button_set_always_show_image(GTK_BUTTON(but_refresh), TRUE);
	gtk_button_set_image(GTK_BUTTON(but_refresh), imag);

	g_signal_connect(G_OBJECT(but_refresh), "clicked", G_CALLBACK(refresh), NULL);
	gtk_box_pack_end(GTK_BOX(box2), but_refresh, FALSE, FALSE, 0);

	GtkWidget *box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(box1), box3, FALSE, FALSE, 0);

	// Tu trzeba jakieś cholerstwo do wyświetlania plansz wstawić

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
