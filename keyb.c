#include "keyb.h"

void input_loop(void)
{
	while(1) {
		char c = getch();
		
		switch(c) {
		case 'q':
			exit(0);

		case 'k':
			move_left(current_buf);
			break;

		case 'l':
			move_right(current_buf);
			break;
		}

		refresh();
	}

}
