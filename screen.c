#include "zalloc.h"
#include "screen.h"
#include "util.h"
#include "keyb.h"

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
	paint_buffer_nlines(buf, LINES - 2);
}

/* paint the first nlines of a buffer */
void paint_buffer_nlines(struct te_buffer *buf, int nlines)
{
	if (buf == NULL)
		return;

	saveyx();
	wmove(buffer_win, 0, 0);

	bstring s;
	int i = 0;
	int count = buf->top_char;

	for(i = 0; i < nlines; i++) {
		s = current_line_as_bstring(buf->contents, count);
		draw_line(s, i);
		count += blength(s) + 1;

		bdestroy(s);
		if (count >= blength(buf->contents))
		    break;
	}

	miniprintf("count %d", count);
	restoreyx();
	refresh();

}

void paint_nthline(struct te_buffer *buf, int n, int y)
{
	if (buf == NULL)
		return;

	saveyx();
	wmove(buffer_win, 0, 0);

	bstring s;
	int i = 0;
	int count = buf->top_char;

	for(i = 0; i < n; i++) {
		s = current_line_as_bstring(buf->contents, count);
		count += blength(s);

		if (count >= blength(buf->contents))
		    break;
	}

	draw_line(s, y);

	restoreyx();
}

void clear_nfirst_lines(WINDOW *w, int n)
{
	int i = 0;
	saveyx();

	for (i = 0; i < n; i++) {
		wmove(w, i, 0);
		wclrtoeol(w);
	}

	restoreyx();
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
		}
/* 		else if (bchar(s, i) == '\n') { */
/* 				mvwaddstr(buffer_win, y, screen_abs++, "\\n"); */
/* 				screen_abs++; */
/* 				mvwaddch(buffer_win, y, screen_abs++, '\n'); */
		/* 		} */ 
		else if (i == COLS - 1) {
			mvwaddch(buffer_win, y, screen_abs, '\\');
			screen_next_line(current_buf);
		} else {
			mvwaddch(buffer_win, y, screen_abs, bchar(s, i));
			screen_abs++;
		}
	}
	restoreyx();
}

void scroll_up(WINDOW *w)
{
	scrollok(w, TRUE);
	wscrl(w, 1);
	touchwin(stdscr); /* We have to use it to scroll the window. */
	scrollok(w, FALSE);
}

void scroll_down(WINDOW *w)
{
	scrollok(w, TRUE);
	wscrl(w, -1);
	touchwin(stdscr); /* We have to use it to scroll the window. */
	scrollok(w, FALSE);
}

void screen_prev_line(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->y > 0) {
		buf->y--;
		buf->x = screen_line_length(buf->contents, buf->point);
	} else {

		if (bstrrchrp(buf->contents, '\n', buf->point) == BSTR_ERR) /* is it the first line of the file ? */
			return;

		scroll_down(buffer_win);
		bstring s = current_line_as_bstring(buf->contents, buf->point - 1);
		draw_line(s, 0);
		bdestroy(s);

		/* update buf->top_char */
		s = current_line_as_bstring(buf->contents, buf->top_char);
		buf->top_char -= blength(s);
		buf->top_char = max(buf->top_char, 0);
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
		scroll_up(buffer_win);
		bstring s = current_line_as_bstring(buf->contents, buf->point - 1);
		if (biseq(bfromcstr(""), s))
			return;

		draw_line(s, LINES - 4);
		bdestroy(s);

		/* update buf->top_char */
		s = current_line_as_bstring(buf->contents, buf->top_char);
		buf->top_char += blength(s);
		buf->top_char = max(buf->top_char, blength(buf->contents));
		bdestroy(s);
	}
}

int screen_move_left(struct te_buffer *buf)
{
	if (buf == NULL)
		return ERR;

	if(move_left(buf) == ERR)
		return ERR;
	
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
	return OK;
}

int screen_move_right(struct te_buffer *buf)
{
	if (buf == NULL)
		return ERR;

	if (move_right(buf) == ERR) /* last char of the document */
		return ERR;

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
	return OK;
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
		if (screen_move_right(buf) == ERR)
			break;
	}

	move(buf->y, buf->x);
	return;
}

void screen_move_down(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->point == blength(buf->contents))
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
		if(screen_move_right(buf) == ERR)
			return;
	}

	move(buf->y, buf->x);
	return;
}

void screen_insert_char(struct te_buffer *buf, char c)
{
	if (buf == NULL)
		return;

	if (buf->dirty < 1)
		buf->dirty = 1;

	insert_char(buf, c);

	if (c == '\n') {
		clrtoeol();
		clear_nfirst_lines(buffer_win, buf->y - 1);
		scroll_down(buffer_win);
		paint_buffer_nlines(buf, buf->y + 1);
		paint_nthline(buf, buf->y + 2, buf->y + 1);
		screen_move_right(buf);
	} else {
		bstring s = current_line_as_bstring(buf->contents, buf->point);
		draw_line(s, buf->y);
		screen_move_right(buf);
	}
}

void screen_delete_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	move_left(buf);
	bstring s = current_line_as_bstring(buf->contents, max(buf->point - 1, 0));
	char c = curr_char(buf);
	delete_char(buf);
	move_right(buf);

	if (buf->dirty < 1)
		buf->dirty = 1;

	if (c == '\n') {
		bstring s2 = current_line_as_bstring(buf->contents, max(buf->point - blength(s), 0));
		statusprintf("s : %s", bstr2cstr(s, '\0'));
		clrtoeol();
		clear_nfirst_lines(buffer_win, buf->y);
		scroll_up(buffer_win);
 		paint_buffer_nlines(buf, buf->y + 1); 
//		screen_prev_line(buf);
		screen_prev_line(buf);
		buf->x = screen_line_length(s, 0);
		move(buf->y, buf->x);
	} else {
		bstring s = current_line_as_bstring(buf->contents, buf->point - 1);
		draw_line(s, buf->y);
		screen_move_left(buf);
		move_right(buf); /* yes it's ugly but I don't feel like recoding screen_move_right atm */
	}

}

void screen_switch_buffer(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	wclear(buffer_win);
	wclear(status_win);
	statusprintf("%s", buf->name);

	paint_buffer(buf);
	move(buf->y, buf->x);
	current_buf = buf;

	return;
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

/*
  notifyprintf : display a message in the minibuffer and wait that the user
  		 presses a key.
 */
void notifyprintf(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	miniprintf(fmt, ap);
	va_end(ap);

	getch(); /* discard input */
}

char *ask_user(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	miniprintf(fmt, ap);
	va_end(ap);

	char *s = zalloc(255);
	int c = 0;
	int i = 0;

	if (s == NULL)
		fail("Unable to allocate memory at __FILE__:__LINE__\n");

	do {
		c = wgetch(minibuffer_win);
		wechochar(minibuffer_win, c);
		s[i] = c;
		i++;
	} while (i < 255 && c != '\n');

	return s;
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

