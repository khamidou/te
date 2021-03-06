#include <stdlib.h>
#include <curses.h>

#include "zalloc.h"
#include "errors.h"
#include "buffer.h"
#include "interp.h"
#include "util.h"
#include "screen.h"


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
	b->top_char = 0;

	return b;
}

void free_buffer(struct te_buffer *b)
{
	if (b == NULL)
		fail("The parameter to free_buffer() is NULL");

	TAILQ_REMOVE(&buffers_head, b, buffers);

	free(b);
}

void free_all_buffers(void)
{
	struct te_buffer *b;
	TAILQ_FOREACH(b, &buffers_head, buffers) {
		free_buffer(b);
	}

}

struct te_buffer* search_buffer(char *name)
{
	if (name == NULL)
		return NULL;

	struct te_buffer *b;
	TAILQ_FOREACH(b, &buffers_head, buffers) {
		if (strcmp(name, b->name) == 0)
			return b;
	}

    return NULL;
}

struct te_buffer* load_buffer(char *filename)
{

	struct te_buffer *b = alloc_and_insert_buffer();

	b->name = strdup(filename);
	b->contents = bfromcstr("");
	if (b->contents == NULL)
		fail("Unable to allocate memory for buffer contents");

	FILE *fp = fopen(filename, "r");

	if (fp == NULL) {
		miniprintf("Creating an empty buffer");
	} else {
		if (breada(b->contents, (bNread) &fread, fp) != 0)
			fail("Unable to read file contents");

		fclose(fp);
	}

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

/* How many chars are there between 0 and X ? */
int screen_numchar_to_offset(bstring s, int x)
{
	int i = 0;

	for (i = 0; i < x; i++)
		if (bchar(s, i) == '\t')
			i += TAB_LEN;
		else
			i += 1;

    return i;
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

	for (i = s_offset + 1; bchar(b, i) != '\n' && i < blength(b); i++) {
		if (bchar(b, i) == '\t')
			count += TAB_LEN;
		else
			count += 1;
	}

	return count;
}

bstring current_line_as_bstring(bstring b, int point)
{
	if (point > blength(b))
		point = blength(b);

	int s_offset = bstrrchrp(b, '\n', max(point, 0));
	if (s_offset == BSTR_ERR)
		s_offset = 0;

	int e_offset = bstrchrp(b, '\n', max(point + 1, 0));
	if (e_offset == BSTR_ERR) {
		e_offset = blength(b);
	}

	if (s_offset == 0) 	/* the first line of the file */
		return bmidstr(b, 0, e_offset);
/* 	else if (e_offset == blength(b)) { */
		
/* 		return bmidstr(b, s_offset + 1, e_offset + 1 - s_offset); */

/* 	} */
	else	{		/* otherwise, skip the first '\n' character */
		bstring s1 = bmidstr(b, s_offset + 1, e_offset - s_offset);
		return s1;
	}
}

/*
  accessors and mutators for the contents of the buffer.

*/

int prev_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return -1;

	return bchar(buf->contents, buf->point - 1);
}

int curr_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return -1;

	return bchar(buf->contents, buf->point);
}

int next_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return -1;

	return bchar(buf->contents, buf->point + 1);
}

int move_left(struct te_buffer *buf)
{
	if (buf == NULL)
		return -1;

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
		return -1;

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
	buf->dirty = 1;
	return;
}

void delete_char(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	bdelete(buf->contents, buf->point, 1);
	move_left(buf);
	buf->dirty = 1;

	return;
}

void write_buffer(struct te_buffer *buf)
{
	if (buf == NULL)
		return;

	char *c = bstr2cstr(buf->contents, '\0');
	FILE *fp = fopen(buf->name, "w+");

	if (fp == NULL)
		notifyprintf("Unable to open %s", buf->name);

	fwrite(c, strlen(c), sizeof(char), fp);

	fclose(fp);
	free(c);
}

/*
  Is the point in the last line ?
*/
int is_last_line(bstring s, int point)
{
	if (bstrchrp(s, '\n', max(point, 0)) == BSTR_ERR)
		return TRUE;
	else
		return FALSE;
}
