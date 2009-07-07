#include <stdlib.h>
#include <curses.h>

#include "zalloc.h"
#include "error.h"
#include "buffer.h"
#include "util.h"

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

	b->name = strdup(filename);
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

	int s_offset = bstrrchrp(b, '\n', point -1);
	if (s_offset == BSTR_ERR)
		s_offset = 0;

	int e_offset = bstrchrp(b, '\n', point);
	if (e_offset == BSTR_ERR)
		e_offset = blength(b);


	return (e_offset - (s_offset + 1)); /* s_offset + 1 because the first '\n' is not a part
					       of the line
					    */
}

/*
  The length of a line as displayed on the screen.
 */
int screen_line_length(bstring b, int point)
{
	if (b == NULL)
		return -1;

	int i = 0;
	int count = 0;

	int s_offset = bstrrchrp(b, '\n', max(point - 1, 0));

	for (i = s_offset + 1; bchar(b, i) != '\n'; i++) {
		if (bchar(b, i) == '\t')
			count += TAB_LEN;
		else
			count += 1;
	}

	return count;
}

bstring current_line_as_bstring(bstring b, int point)
{
	int s_offset = bstrrchrp(b, '\n', max(point, 0));
	if (s_offset == BSTR_ERR)
		s_offset = 0;

	int e_offset = bstrchrp(b, '\n', point + 1);
	if (e_offset == BSTR_ERR)
		e_offset = blength(b);

	if (s_offset == 0) /* the first line of the file */
		return bmidstr(b, s_offset, e_offset - s_offset);
	else 
		return bmidstr(b, s_offset + 1, e_offset - s_offset);
}

/*
  accessors and mutators for the contents of the buffer.

*/

int prev_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	return bchar(buf->contents, buf->point - 1);
}

int curr_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	return bchar(buf->contents, buf->point);
}

int next_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	return bchar(buf->contents, buf->point + 1);
}

int move_left(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->point > 0) {
		buf->point--;
		if (bchar(buf->contents, buf->point) == '\n')
			decrement_or_nullify(buf->lineno);
	}
	else
		return ERR;

	return OK;
}


int move_right(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	if (buf->point < blength(buf->contents)) {
		buf->point++;
		if (prev_char(buf) == '\n')
			buf->lineno++;
	}
	else
		return ERR;

	return OK;
}

void move_to_prev_line(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	while(curr_char(buf) != '\n')
		move_left(buf);

}

void move_to_next_line(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	while(curr_char(buf) != '\n')
		move_right(buf);

	move_right(buf);
}

void insert_char(struct te_buffer *buf, char c)
{
	if (buf == NULL)
		return;

	binsertch(buf->contents, buf->point, 1, c);

	return;
}


