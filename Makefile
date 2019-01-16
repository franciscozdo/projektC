CC=gcc
GTKFLAGS = $(shell pkg-config gtk+-3.0 --cflags --libs)
CFLAGS=-std=c11 -Wall -Wextra -Wno-unused-parameter 
#GTKFLAGS=-pthread -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/libdrm -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0

NAME=battleships

SRC=window.c game.c msg.c lin-fifo.c boards.c #creat.c
DEPS=game.h msg.h fifo.h boards.h #creat.h
OBJ=game.o msg.o lin-fifo.o window.o boards.o #creat.o

#YOU: $(SRC) $(NAME)

$(NAME): $(OBJ) $(DEPS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(GTKFLAGS)

window.o: window.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(GTKFLAGS) 

#creat.o: creat.c $(DEPS)
#	$(CC) $(CFLAGS) -c -o $@ $< $(GTKFLAGS) 


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(NAME)

#game.o: game.c
#	$(CC) $(CFLAGS) -c -o $@ $<

#lin-fifo.o: lin-fifo.c
#	$(CC) $(CFLAGS) -c -o $@ $<

#msg.o: msg.c
#	$(CC) $(CFLAGS) -c -o $@ $<

