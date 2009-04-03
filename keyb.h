#ifndef __KEYB_H__
#define __KEYB_H__

#include <stdlib.h>
#include "buffer.h"
#include "screen.h"

void input_loop(void);

struct te_buffer *current_buf;

#endif
