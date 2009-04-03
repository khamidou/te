#include <stdlib.h>
#include <curses.h>

#include "zalloc.h"
#include "error.h"
#include "buffer.h"

void init_buffers(void)
{
	TAILQ_INIT(&buffers_head);
}

struct te_char *alloc_and_insert_char(struct te_buffer *buf, char contents)
{
	if (buf == NULL)
		fail("The buffer parameter to alloc_and_insert_char is NULL");

	struct te_char *c = zalloc(sizeof(struct te_buffer));

	if (c == NULL)
		fail("Unable to allocate memory for te_char struct");

	TAILQ_INSERT_TAIL(&buf->chars_head, c, chars);

	c->contents = contents;
	return c;
}

struct te_buffer *alloc_and_insert_buffer(void)
{
	struct te_buffer *b = zalloc(sizeof(struct te_buffer));
	
	if (b == NULL)
		fail("Unable to allocate memory for te_buffer struct");

	TAILQ_INIT(&b->chars_head);
	TAILQ_INSERT_TAIL(&buffers_head, b, buffers);

	return b;
}

struct te_char *nth_char(struct te_char *c, int n)
{
	if (c == NULL)
		return;

	struct te_char *r = c;
	int i = 0;

	for(i = 0; r != NULL && i < n; i++)
		r = TAILQ_NEXT(c, chars);

	statusprintf("r is %x, r->contents are : %c", r, r->contents);
	
	return r;
}

void free_char(struct te_buffer *b, struct te_char *c)
{
	if (b == NULL || c == NULL)
		fail("One of the parameters to free_char() is NULL");

	TAILQ_REMOVE(&b->chars_head, c, chars);

	free(c);
}

void free_buffer(struct te_buffer *b)
{
	if (b == NULL)
		fail("The parameter to free_buffer() is NULL");

	TAILQ_REMOVE(&buffers_head, b, buffers);

	free(b);
}


struct te_buffer* load_buffer(char *filename)
{
	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
		fail("Unable to open %s", filename);

	struct te_buffer *b = alloc_and_insert_buffer();

	char c = getc(fp);

	while(!feof(fp)) {
		alloc_and_insert_char(b, c);
		c = getc(fp);
	}

	statusprintf("%s", filename);

	b->point = TAILQ_FIRST(&b->chars_head);
	b->scr_top = b->point;
	b->scr_bottom = nth_char(b->point, (LINES - 3) * COLS);

	return b;
}

void scroll_up(struct te_buffer *buf)
{

}

void scroll_down(struct te_buffer *buf)
{
	if (buf == NULL || buf->point == NULL)
		return;

	struct te_char *c = buf->point;
	
	
}

void move_left(struct te_buffer *buf)
{

}

void move_right(struct te_buffer *buf)
{

}

void move_up(struct te_buffer *buf)
{

}

void move_down(struct te_buffer *buf)
{

}

