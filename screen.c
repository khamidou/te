#include "screen.h"
#include "util.h"

static int scr_y, scr_x;


#define saveyx() getyx(stdscr, scr_y, scr_x)
#define restoreyx() move(scr_y, scr_x)

void init_windows(void)
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	scrollok(stdscr, FALSE);

	buffer_win = subwin(stdscr, LINES - 3, COLS, 0, 0);
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

	saveyx();
	wmove(buffer_win, 0, 0);

	bstring s;
	int i = 0;
	int count = 0;

	for(i = 0; i < (LINES - 2); i++) {
		s = current_line_as_bstring(buf->contents, count);
		draw_line(s, i);
		count += blength(s);

		if (count >= blength(buf->contents))
		    break;
	}
	
	restoreyx();
	refresh();
}

void draw_line(bstring s, int y)
{
	int i;
	int j;
	int screen_abs = 0;
	saveyx();
	move(y, 0);
	clrtoeol();

	/* FIXME : wrap line if we go beyond COLS */
	for (i = 0; i < blength(s); i++) {
		if (bchar(s, i) == '\t') {
			for (j = 0; j < TAB_LEN; j++)
				mvwaddch(buffer_win, y, screen_abs++, ' ');
		} else {
			mvwaddch(buffer_win, y, screen_abs, bchar(s, i));
			screen_abs++;
		}
	}
	restoreyx();
}

void scroll_up(struct te_buffer *buf)
{

}

void scroll_down(struct te_buffer *buf)
{

}

void screen_prev_line(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->y > 0) {
		buf->y--;
		buf->x = screen_line_length(buf->contents, buf->point);
	} else {
		/* scroll up */
		scrollok(buffer_win, TRUE);
		wscrl(buffer_win, -1);
		touchwin(stdscr); /* We have to use it to scroll the window. */
		scrollok(buffer_win, FALSE);
		bstring s = current_line_as_bstring(buf->contents, buf->point - 1);
		draw_line(s, 0);
		bdestroy(s);
	}
}

void screen_next_line(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	buf->x = 0;
	if (buf->y < LINES - 4)
		buf->y++;
	else {
		scrollok(buffer_win, TRUE);
		wscrl(buffer_win, 1);
		touchwin(stdscr); /* We have to use it to scroll the window. */
		scrollok(buffer_win, FALSE);
		bstring s = current_line_as_bstring(buf->contents, buf->point - 1);
		draw_line(s, LINES - 4);
		bdestroy(s);
	}
}

void screen_move_left(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if(move_left(buf) == ERR)
		return;
	
	if (curr_char(buf) == '\n') {
			screen_prev_line(buf);
			buf->x = screen_line_length(buf->contents, buf->point);
	} else { 
		if (buf->x > 0)
			if (curr_char(buf) == '\t') {
				buf->x -= TAB_LEN;
			} else {
					buf->x--;
			}
	}

	move(buf->y, buf->x);
}

void screen_move_right(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (move_right(buf) == ERR) /* last char of the document */
		return;

	if (prev_char(buf) == '\n') {
		screen_next_line(buf);
	} else { 
		/* tab is the only character larger than 1 */
			if (prev_char(buf) == '\t') {
				buf->x += TAB_LEN;
			} else {
				buf->x++;
			}
	}

	move(buf->y, buf->x);
}

void screen_move_up(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	int	i = 0;
	int	old_x = buf->x;

	/* move until the first character of the line */
	while(prev_char(buf) != '\n') {

		if(move_left(buf) == ERR)
			break;
	} 
	move_left(buf);

	/* then, move to the beginning of the previous line */
	while(prev_char(buf) != '\n') {

		if(move_left(buf) == ERR)
			break;
	}

 	screen_prev_line(buf);
 	buf->x = 0; 

	/* mimic emacs' behaviour of moving the user to the exact offset we were on */
		
	while(buf->x < old_x && next_char(buf) != '\n' && curr_char(buf) != '\n') {
		screen_move_right(buf);
	}

	move(buf->y, buf->x);
	return;
}

void screen_move_down(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	int	i = 0;
	int	old_x = buf->x;

	do {
		/* move until the first char of the next line */
		if (move_right(buf) == ERR)
			break;

	} while (prev_char(buf) != '\n');

	screen_next_line(buf);

	/* mimic emacs' behaviour of moving the user to the exact offset we were on */
	while (buf->x < old_x && curr_char(buf) != '\n') {
		screen_move_right(buf);
	}

	move(buf->y, buf->x);
	return;
}

void screen_insert_char(struct te_buffer *buf, char c)
{
	if (buf == NULL)
		return;

	insert_char(buf, c);
	
	bstring s = current_line_as_bstring(buf->contents, buf->point);
	draw_line(s, buf->y);
	screen_move_right(buf);
	
}

/*
  display a message in the status bar.
 */
void statusprintf(char *fmt, ...)
{
	
	saveyx();

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
	
	restoreyx();

}


void miniprintf(char *fmt, ...)
{
	saveyx();

	va_list ap;
	va_start(ap, fmt);
	werase(minibuffer_win);
	vwprintw(minibuffer_win, fmt, ap);
	va_end(ap);

	wrefresh(minibuffer_win);

	restoreyx();
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

