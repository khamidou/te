#include "keyb.h"

void input_loop(void)
{
	while(1) {
		char c = getch();
		
		switch(c) {
		case 'q':
			exit(0);

		case 'k':
		case KEY_LEFT:
			move_left(current_buf);
			break;

		case 'l':
		case KEY_RIGHT:
			move_right(current_buf);
			break;
		}

		refresh();
	}

}
