/*
  interp.h : bindings for the S-lang interpreter.
 */
#ifndef __INTERP_H__
#define __INTERP_H__

#include <stdlib.h>
#include <slang.h>
#include "buffer.h"

void intrin_statusprintf(char **s);
void intrin_miniprintf(char **s);

SLang_Intrin_Fun_Type te_intrinsics [] = 
{
	/* table entries */
	MAKE_INTRINSIC_1("miniprintf", intrin_miniprintf, SLANG_VOID_TYPE, SLANG_STRING_TYPE),
	MAKE_INTRINSIC_1("statusprintf", intrin_statusprintf, SLANG_VOID_TYPE, SLANG_STRING_TYPE),

	SLANG_END_INTRIN_FUN_TABLE
};

SLang_CStruct_Field_Type buffer_type_layout [] =
{
	MAKE_CSTRUCT_FIELD(struct te_buffer, name, "name", SLANG_STRING_TYPE, 0),
	MAKE_CSTRUCT_FIELD(struct te_buffer, point, "point", SLANG_INT_TYPE, 0),
	MAKE_CSTRUCT_FIELD(struct te_buffer, dirty, "dirty", SLANG_INT_TYPE, 0),

      SLANG_END_CSTRUCT_TABLE
};

#endif
