/*
  te, a small text editor for unix.
 */

#include <stdio.h>
#include <curses.h>


int main(int argc, char **argv)
{
	init_buffers();
	init_windows();

	struct te_buffer *b = load_buffer("screen.c");
	paint_buffer(b);
	miniprintf("Over the top!");



	input_loop();

}

