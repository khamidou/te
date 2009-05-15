#include "screen.h"

void init_windows(void)
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	buffer_win = subwin(stdscr, LINES - 2, COLS, 0, 0);
	idlok(stdscr, TRUE);
	scrollok(buffer_win, FALSE);

	status_win = subwin(stdscr, 1, COLS, LINES - 2, 0);
	wattron(status_win, A_REVERSE);

	minibuffer_win = subwin(stdscr, 1, COLS, LINES - 1, 0);

	if (signal(SIGWINCH, console_signal_handler) == SIG_ERR)
		fail("Unable to set up the SIGWINCH signal handler");

	atexit(&cleanup_windows);
}

void cleanup_windows(void)
{
	endwin();
}

void paint_buffer(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	wmove(buffer_win, 0, 0);

	char c;
	int i = 0;

	for(i = 0; i < (LINES - 3) * COLS; i++) 
		waddch(buffer_win, bchar(buf->contents, i));
	
	wmove(buffer_win, 0, 0);
	refresh();
}


void scroll_up(struct te_buffer *buf)
{

}

void scroll_down(struct te_buffer *buf)
{

}

void move_left(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->x > 0)
		if (bchar(buf->contents, buf->point) == '\t')
			buf->x -= TAB_LEN;
		else
			buf->x--;

	else if (buf->y > 0) {
		buf->y--;
		buf->x = screen_line_length(buf->contents, buf->point);
	}

	prev_char(buf);
	move(buf->y, buf->x);
}

void move_right(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (next_char(buf) == ERR) /* last char of the document */
		return;

	if (buf->x < screen_line_length(buf->contents, buf->point)) {
		/* tab is the only character larger than 1 */
		if (bchar(buf->contents, buf->point) == '\t')
			buf->x += TAB_LEN;
		else
			buf->x++;
	}
	else if (buf->y < LINES - 3) {
		buf->y++;
		buf->x = 0;
	}


	move(buf->y, buf->x);
	refresh();
	miniprintf("%c, y: %d, x%d", bchar(buf->contents, buf->point), buf->y, buf->x);
}

void move_up(struct te_buffer *buf)
{

}

void move_down(struct te_buffer *buf)
{

}

void statusprintf(char *fmt, ...)
{
	werase(status_win);
	wmove(status_win, 0, 0);

	va_list ap;
	va_start(ap, fmt);
	vwprintw(status_win, fmt, ap);
	va_end(ap);

	/* We have to clear the screen to the end of line because for some unexpected
	   reason, curses doesn't want to do it
	*/

	int y, x;
	getyx(stdscr, y, x);
	while(x <= COLS) {
		waddch(status_win, ' ');
		x++;
	}

	wrefresh(status_win);
}


void miniprintf(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	werase(minibuffer_win);
	vwprintw(minibuffer_win, fmt, ap);
	va_end(ap);

	wrefresh(minibuffer_win);
}

void console_signal_handler(int sig)
{
	switch(sig) {
	case SIGWINCH:
	default:
		refresh();
		break;

	}
}
