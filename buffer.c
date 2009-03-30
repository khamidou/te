#include <stdlib.h>

#include "zalloc.h"
#include "error.h"
#include "buffer.h"

struct te_char *alloc_and_insert_char(struct te_buffer *buf)
{
	if (buf == NULL)
		fail("The buffer parameter to alloc_and_insert_char is NULL");

	struct te_char *c = zalloc(sizeof(struct te_buffer));

	if (c == NULL)
		fail("Unable to allocate memory for te_char struct");

	TAILQ_INSERT_TAIL(&buf->chars_head, c, chars);

	return c;
}

struct te_buffer *alloc_and_insert_buffer(void)
{
	struct te_buffer *b = zalloc(sizeof(struct te_buffer));
	
	if (b == NULL)
		fail("Unable to allocate memory for te_buffer struct");

	TAILQ_INSERT_TAIL(&buffers_head, b, buffers);

	return b;
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
