/*
  te, a small text editor for unix.
*/

#include <stdio.h>
#include <curses.h>
#include "python-bindings.h"
#include "screen.h"

extern struct te_buffer *current_buf;

int main(int argc, char **argv)
{
	init_python();
	init_buffers();
	init_windows();

	current_buf = load_buffer("README");
	paint_buffer(current_buf);
	refresh();
	input_loop();

}

