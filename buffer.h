#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdio.h>
#include "bstrlib.h"
#include "queue.h"


struct te_buffer {
	char *name;
	int dirty;
	int x, y;
	bstring contents;
	int point;		/* offset in "contents" */
	struct te_char *scr_top, *scr_bottom; /* the first and last char of the screen */

	TAILQ_HEAD(te_chars_head, te_char) chars_head;
	TAILQ_ENTRY(te_buffer) buffers;
};

TAILQ_HEAD(te_buffers_head, te_buffer) buffers_head;

struct te_char 			*alloc_and_insert_char(struct te_buffer *buf, char contents);
struct	te_buffer 		*alloc_and_insert_buffer(void);
struct te_char 			*nth_char(struct te_char *c, int n);
void 				free_char(struct te_buffer *b, struct te_char *c);
void				free_buffer(struct te_buffer *b);
struct te_buffer* 		load_buffer(char *filename);

void 				init_buffers(void);
void				prev_char(struct te_buffer *buf);
void				next_char(struct te_buffer *buf);



#endif
