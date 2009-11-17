/*
  te, a small text editor for unix.
*/

#include <stdio.h>
#include <curses.h>
#include "interp.h"
#include "screen.h"

void init_interp(void);

extern struct te_buffer *current_buf;

int main(int argc, char **argv)
{
	init_buffers();
	init_windows();
	init_python();

	if (argc != 1)
		current_buf = load_buffer(argv[1]);
	else 
		current_buf = load_buffer("tests/bufferfuncs.c");

	load_buffer("README");
	load_buffer("main.c");
	
	paint_buffer(current_buf);
	run_python_script("te_test");
	refresh();
	input_loop();

}

