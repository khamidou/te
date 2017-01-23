#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdio.h>
#include "interp.h"
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
	int top_char; /* the position of the char at the top of the screen */
	TAILQ_ENTRY(te_buffer) buffers;
};

TAILQ_HEAD(te_buffers_head, te_buffer) buffers_head;

struct	te_buffer 		*alloc_and_insert_buffer(void);
void				free_buffer(struct te_buffer *b);
void 				free_all_buffers(void);
struct te_buffer* 		search_buffer(char *name);
struct te_buffer* 		load_buffer(char *filename);

void 				init_buffers(void);
int 				line_length(bstring b, int point);
int 				screen_numchar_to_offset(bstring s, int x);
int 				screen_line_length(bstring b, int point);
bstring				current_line_as_bstring(bstring b, int point);
int				prev_char(struct te_buffer *buf);
int 				curr_char(struct te_buffer *buf);
int				next_char(struct te_buffer *buf);
int 				move_right(struct te_buffer *buf);
int 				move_left(struct te_buffer *buf);
void 				insert_char(struct te_buffer *buf, char c);
void 				delete_char(struct te_buffer *buf);
int 				is_last_line(bstring s, int point);
void                write_buffer(struct te_buffer *buf);

#endif
