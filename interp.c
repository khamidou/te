#include "interp.h"
#include "screen.h"

void init_interp(void)
{
	if (SLang_init_all () == - 1)
		exit (EXIT_FAILURE);

	if (SLadd_intrin_fun_table(te_intrinsics, NULL) == -1)
	{
		fail("Unable to load the S-lang interface - exiting.");
	}

	if (SLang_load_file ("tests/test.sl") == -1)
	{
		/* Clear the error and reset the interpreter */
		SLang_restart (1);
	}

/* TODO : load the file named .terc. */
/* 	if (SLang_load_file ("site.sl") == -1) */
/* 	{ */
/* 		/\* Clear the error and reset the interpreter *\/ */
/* 		SLang_restart (1); */
/* 	} */

}

void intrin_miniprintf(char **s)
{
	miniprintf("%s", *s);
}

void intrin_statusprintf(char **s)
{
	statusprintf("%s", *s);
}
