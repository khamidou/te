#include "screen.h"

void init_windows(void)
{
	initscr();
	cbreak();
	noecho();
	scrollok(stdscr, FALSE);
	keypad(stdscr, TRUE);

	buffer_win = subwin(stdscr, LINES - 2, COLS, 0, 0);

	status_win = subwin(stdscr, 1, COLS, LINES - 2, 0);
	wattron(status_win, A_REVERSE);

	minibuffer_win = subwin(stdscr, 1, COLS, LINES - 1, 0);

	if (signal(SIGWINCH, console_signal_handler) == SIG_ERR)
		fail("Unable to set up the SIGWINCH signal handler");
}

void cleanup_windows(void)
{
	endwin();
}

void paint_buffer(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	struct te_char *c;
	int y, x;

	TAILQ_FOREACH(c, &buf->chars_head, chars) {
		getyx(stdscr, y, x);
		if (y >= LINES -2)
			break;

		waddch(buffer_win, c->contents);
	}
	
	refresh();
}

void screen_move_left(void)
{

}

void screen_move_right(void)
{

}

void screen_move_up(void)
{

}

void screen_move_down(void)
{

}

void statusprintf(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vwprintw(status_win, fmt, ap);
	va_end(ap);

	wclrtoeol(status_win);
	wrefresh(status_win);
}


void miniprintf(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
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
