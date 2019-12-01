CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0)

skwin: src/skwin.c
	gcc $(CFLAGS) -o skwin src/skwin.c $(LIBS)
