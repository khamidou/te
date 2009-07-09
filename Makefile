CC=gcc
CFLAGS=-c -g `lua-config --include`
LDFLAGS=-lcurses `lua-config --libs`
OBJS=main.o errors.o screen.o buffer.o keyb.o bstrlib.o lua-api.o

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

lua-api.o: lua-api.c
	$(CC) $(CFLAGS) lua-api.c

clean: 
	rm $(OBJS)
	rm *~
	rm te