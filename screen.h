#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdarg.h>
#include <signal.h>
#include <curses.h>

#include "buffer.h"
#include "config.h"
#include "util.h"

WINDOW *buffer_win, *status_win, *minibuffer_win;

void init_windows(void);
void cleanup_windows(void);

void paint_buffer(struct te_buffer *buf);
void paint_buffer_nlines(struct te_buffer *buf, int nlines);
void paint_nthline(struct te_buffer *buf, int n, int y);
void clear_nfirst_lines(WINDOW *w, int n);
void draw_line(bstring s, int y);
void scroll_up(WINDOW *w);
void scroll_down(WINDOW *w);

int  screen_move_left(struct te_buffer *buf);
int  screen_move_right(struct te_buffer *buf);
void screen_move_up(struct te_buffer *buf);
void screen_move_down(struct te_buffer *buf);
void screen_delete_char(struct te_buffer *buf);
void screen_insert_char(struct te_buffer *buf, char c);

void statusprintf(char *fmt, ...);
void miniprintf(char *fmt, ...);
void notifyprintf(char *fmt, ...);
char *ask_user(char *fmt, ...);

void console_signal_handler(int sig);

#endif
