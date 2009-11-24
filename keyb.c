#include "keyb.h"

int command_mode = 1;  /* command mode = 1, input mode = 0 */
char *str;

void input_loop(void)
{
	while(1) {

		int c = getch();
		
		if (command_mode)
			process_command(c);
		else 
			process_input(c);

		refresh();

/* 		miniprintf("%c, line length: %d, y: %d, x%d", */
/* 			   (bchar(current_buf->contents, current_buf->point) == '\n') ? 'N' : bchar(current_buf->contents, current_buf->point), */
/* 			   screen_line_length(current_buf->contents, current_buf->point), */
/* 			   current_buf->y, current_buf->x); */

	}

}


void process_command(int c)
{
			switch(c) {
			case 'q':
				exit(0);

			case 'h':
			case KEY_LEFT:
				screen_move_left(current_buf);
				break;

			case 'l':
			case KEY_RIGHT:
				screen_move_right(current_buf);
				break;

			case 'j':
			case KEY_DOWN:
				screen_move_down(current_buf);
				break;

			case 'k':
			case KEY_UP:
				screen_move_up(current_buf);
				break;

			case 'w':
				write_buffer(current_buf);
				break;

			case 'x':
				screen_delete_char(current_buf);
				break;

			case 'i':
			case KEY_END:
				/* switch between command and input mode */
				command_mode = 0;
				miniprintf("-- INPUT --");
				break;

			case 'v':

			case 'b':
			default:
				miniprintf("%c - invalid command", c);
				break;
			}
}

void process_input(int c)
{
			switch(c) {
			case KEY_ENTER:
				screen_insert_char(current_buf, '\n');
				break;

			case KEY_BACKSPACE:
				screen_delete_char(current_buf);
				break;

			case KEY_LEFT:
				screen_move_left(current_buf);
				break;

			case KEY_RIGHT:
				screen_move_right(current_buf);
				break;

			case KEY_DOWN:
				screen_move_down(current_buf);
				break;

			case KEY_UP:
				screen_move_up(current_buf);
				break;
				
			case KEY_END:
				command_mode = 1;
				miniprintf("-- COMMAND --");
				break;

			default:
				screen_insert_char(current_buf, c);
				refresh();
				break;
			}

}
