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

	struct te_char *c;
	int i = 0;

	for(c = buf->scr_top; c != buf->scr_bottom; c = TAILQ_NEXT(c, chars)) {
		if (++i > (LINES - 3) * COLS)
			break;
		else
			waddch(buffer_win, c->contents);
	}
	miniprintf("%d", i);
	wmove(buffer_win, 0, 0);
	refresh();
}

void screen_move_left(struct te_buffer *buf)
{

}

void screen_move_right(struct te_buffer *buf)
{

}

void screen_move_up(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->scr_top == NULL || buf->scr_bottom == NULL)
		fail("FATAL: buf->scr_top or buf->scr_bottom is NULL\n");

	scrollok(buffer_win, TRUE);
	wscrl(buffer_win, -1);
	
	wmove(buffer_win, LINES - 3, 0);
	struct te_char *c = buf->scr_bottom;

	for (c = buf->scr_bottom; c != NULL && c->contents != '\n'; c = TAILQ_NEXT(c, chars))
		addch(c->contents);
	
	buf->scr_bottom = c;

	/* Update scr_top accordingly */
	c = buf->scr_top;
	for (c = buf->scr_top; c != NULL && c->contents != '\n'; c = TAILQ_NEXT(c, chars))
		continue;
	
	buf->scr_top = c;

	scrollok(buffer_win, FALSE);

	wrefresh(buffer_win);
}

void screen_move_down(struct te_buffer *buf)
{
	scroll(buffer_win);
	wrefresh(buffer_win);
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
