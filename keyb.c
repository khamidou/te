#include "keyb.h"

void input_loop(void)
{
	while(1) {
		int c = getch();
		
		switch(c) {
		case 'q':
			exit(0);

		case 'k':
		case KEY_LEFT:
			screen_move_left(current_buf);
			break;

		case 'l':
		case KEY_RIGHT:
			screen_move_right(current_buf);
			break;

		case 'd':
		case KEY_DOWN:
			screen_move_down(current_buf);
			break;
			
		case 'v':
			screen_insert_char(current_buf, 'X');
			paint_buffer(current_buf);
			break;
		}
		
		refresh();

		miniprintf("%c, line length: %d, y: %d, x%d", 
			   (bchar(current_buf->contents, current_buf->point) == '\n') ? 'N' : bchar(current_buf->contents, current_buf->point), 
			   screen_line_length(current_buf->contents, current_buf->point),
			   current_buf->y, current_buf->x);

	}

}
