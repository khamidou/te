CC=gcc
CFLAGS=-c -g
LDFLAGS=-lcurses
OBJS=main.o errors.o screen.o buffer.o keyb.o interp.o bstrlib.o

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

interp.o: interp.c
	$(CC) $(CFLAGS) interp.c

bstrlib.o: bstrlib.c
	$(CC) $(CFLAGS) bstrlib.c

clean:
	rm $(OBJS)
	rm te
