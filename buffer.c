#include <stdlib.h>
#include <curses.h>

#include "zalloc.h"
#include "error.h"
#include "buffer.h"

void init_buffers(void)
{
	TAILQ_INIT(&buffers_head);
}

struct te_buffer *alloc_and_insert_buffer(void)
{
	struct te_buffer *b = zalloc(sizeof(struct te_buffer));
	
	if (b == NULL)
		fail("Unable to allocate memory for te_buffer struct");

	TAILQ_INSERT_TAIL(&buffers_head, b, buffers);

	b->x = 0;
	b->y = 0;

	return b;
}

struct te_char *nth_char(struct te_char *c, int n)
{
}

void free_char(struct te_buffer *b, struct te_char *c)
{
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

	b->contents = bfromcstr("");
	if (b->contents == NULL)
		fail("Unable to allocate memory for buffer contents");

	if (breada(b->contents, (bNread) &fread, fp) != 0)
		fail("Unable to read file contents");
	
	statusprintf("%s", filename);

	return b;
}

void prev_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->point > 0)
		buf->point--;
}

void next_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->point < blength(buf->contents))
		buf->point++;

}


