#include "keyb.h"

void input_loop(void)
{
	while(1) {
		int c = getch();
		
		switch(c) {
		case 'q':
			exit(0);

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

		case KEY_BACKSPACE:
			screen_delete_char(current_buf);
			break;

		case 'u':
			wscrl(buffer_win, -10);
			touchwin(stdscr);
			break;
		default:
			screen_insert_char(current_buf, c);
			refresh();
			break;
		}
		
		refresh();

		miniprintf("%c, line length: %d, y: %d, x%d",
			   (bchar(current_buf->contents, current_buf->point) == '\n') ? 'N' : bchar(current_buf->contents, current_buf->point),
			   screen_line_length(current_buf->contents, current_buf->point),
			   current_buf->y, current_buf->x);

	}

}
