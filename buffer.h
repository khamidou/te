#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "queue.h"

struct te_char {
	char contents;
	TAILQ_ENTRY(te_char) chars;
};

struct te_buffer {
	char *name;
	int dirty;
	TAILQ_HEAD(te_chars_head, te_char) chars_head;
	TAILQ_ENTRY(te_buffer) buffers;
};

TAILQ_HEAD(te_buffers_head, te_buffer) buffers_head;

#endif
