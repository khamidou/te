#ifndef __KEYB_H__
#define __KEYB_H__

#include <stdlib.h>
#include "buffer.h"
#include "screen.h"

void input_loop(void);
void process_command(int c); /* process a command */
void process_input(int c);   /* process a normal char - that is, not a command */

struct te_buffer *current_buf;
struct te_buffer *buf1;
struct te_buffer *buf2;

#endif
