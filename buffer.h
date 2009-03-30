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

struct te_char 		*alloc_and_insert_char(struct te_buffer *buf);
struct te_buffer 	*alloc_and_insert_buffer(void);
void 			free_char(struct te_buffer *b, struct te_char *c);
void			free_buffer(struct te_buffer *b);

void init_buffers();

#endif
