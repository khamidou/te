/*
  te, a small text editor for unix.
*/

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "interp.h"
#include "screen.h"

void init_interp(void);
void cmdline_help(void);

extern struct te_buffer *current_buf;

int main(int argc, char **argv)
{


	if (argc <= 1) 
		cmdline_help();

	init_windows();
	init_buffers();

	for (argv++; *argv != NULL ; argv++) 
		current_buf = load_buffer(*argv);
		

	paint_buffer(current_buf);
	statusprintf("%s", current_buf->name);

	refresh();
	input_loop();

}

void cmdline_help(void)
{
	puts("te file1 file2");
	exit(EXIT_SUCCESS);
}
