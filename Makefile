CC=gcc
CFLAGS=-c
LDFLAGS=-lcurses
OBJS=main.o errors.o screen.o buffer.o

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

