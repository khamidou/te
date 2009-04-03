#include "keyb.h"

void input_loop(void)
{
	while(1) {
		char c = getch();
		
		switch(c) {
		case 'q':
			exit(0);

		case 'k':
			screen_move_down(current_buf);
			break;
		}
	}

}
