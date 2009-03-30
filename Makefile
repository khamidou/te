CC=gcc
CFLAGS=-c
LDFLAGS=-lcurses
OBJS=main.o

te: $(OBJS)
	$(CC) $(LDFLAGS) -o te $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) main.c

