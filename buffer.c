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
	b->point = 0;

	return b;
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

/* Compute the length of a string between two '\n' */
int line_length(bstring b, int point)
{
	if (b == NULL)
		return -1;

	int c_offset = bstrchrp(b, '\n', point);
	if (c_offset == BSTR_ERR)
		c_offset = 0;

	int p_offset = bstrrchrp(b, '\n', c_offset - 1);
	if (p_offset == BSTR_ERR)
		p_offset = 0;

	return c_offset - p_offset;
}

/*
  The length of a line as displayed on the screen.
 */
int screen_line_length(bstring b, int point)
{
	if (b == NULL)
		return -1;

	int len = line_length(b, point);
	int i = 0;
	int count = 0;

	for (i = 0; i < len; i++) {
		if (bchar(b, point - i) == '\t')
			count += TAB_LEN;
		else
			count += 1;
	}

	return count;
}

int prev_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->point > 0)
		buf->point--;
	else
		return ERR;

	return OK;
}

int next_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->point < blength(buf->contents))
		buf->point++;
	else
		return ERR;

	return OK;
}


