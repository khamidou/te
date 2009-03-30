#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdarg.h>
#include <signal.h>
#include <curses.h>

#include "buffer.h"

WINDOW *buffer_win, *status_win, *minibuffer_win;

void init_windows(void);
void cleanup_windows(void);

void paint_buffer(struct te_buffer *buf);
void screen_move_left(void);
void screen_move_right(void);
void screen_move_up(void);
void screen_move_down(void);

void statusprintf(char *fmt, ...);
void miniprintf(char *fmt, ...);

void console_signal_handler(int sig);

#endif
