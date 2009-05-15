#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdarg.h>
#include <signal.h>
#include <curses.h>

#include "buffer.h"
#include "config.h"


WINDOW *buffer_win, *status_win, *minibuffer_win;

void init_windows(void);
void cleanup_windows(void);

void paint_buffer(struct te_buffer *buf);

void scroll_up(struct te_buffer *buf);
void scroll_down(struct te_buffer *buf);
void move_left(struct te_buffer *buf);
void move_right(struct te_buffer *buf);
void move_up(struct te_buffer *buf);
void move_down(struct te_buffer *buf);

void statusprintf(char *fmt, ...);
void miniprintf(char *fmt, ...);

void console_signal_handler(int sig);

#endif
