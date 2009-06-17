#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdio.h>
#include <Python.h>
#include "bstrlib.h"
#include "queue.h"
#include "config.h"

struct te_buffer {
	char *name;
	int dirty;
	int x, y;
	int lineno; /* the number of '\n' seen so far */
	bstring contents;
	int point;		/* offset in "contents" */
	struct te_char *scr_top, *scr_bottom; /* the first and last char of the screen */

	PyObject *pyBuffer;	/* pointer to the buffer object */

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
int 				line_length(bstring b, int point);
int 				screen_line_length(bstring b, int point);
bstring				current_line_as_bstring(bstring b, int point);
int				prev_char(struct te_buffer *buf);
int 				curr_char(struct te_buffer *buf);
int				next_char(struct te_buffer *buf);
int 				move_right(struct te_buffer *buf);
int 				move_left(struct te_buffer *buf);
void 				insert_char(struct te_buffer *buf, char c);

#endif
