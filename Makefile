CC=gcc
CFLAGS=-c -g `python2.5-config --includes`
LDFLAGS=-lcurses `python2.5-config --ldflags`
OBJS=main.o errors.o screen.o buffer.o keyb.o bstrlib.o python-bindings.o

te: $(OBJS)
	$(CC) $(LDFLAGS) -o te $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) main.c

error.o: error.c
	$(CC) $(CFLAGS) error.c

screen.o: screen.c
	$(CC) $(CFLAGS) screen.c

buffer.o: buffer.c
	$(CC) $(CFLAGS) buffer.c

keyb.o: keyb.c
	$(CC) $(CFLAGS) keyb.c

bstrlib.o: bstrlib.c
	$(CC) $(CFLAGS) bstrlib.c

python-bindings.o: python-bindings.c python-bindings.h
	$(CC) $(CFLAGS) python-bindings.c

clean: $(OBJS)
	rm $(OBJS)
	rm te