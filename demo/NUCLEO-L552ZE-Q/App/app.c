/*
 * app.c
 *
 *  Created on: Dec 5, 2021
 *      Author: john
 */

#include <stdio.h>
#include "main.h"
#include "vt100.h"
#include "tcli.h"

void app_init(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	puts("TinyCLI Demo\n");
}

static char buf[256];

void app_run(void)
{
	int c;
	static int buf_idx = -1;
	int rc;

	if (buf_idx < 0)
	{
		putchar('>');
		buf_idx = 0;
	}

	if ((c = getchar()) == EOF)
	{
		return;
	}

	if (c >= ' ' && c < CHAR_DEL && buf_idx < (sizeof(buf) - 2))
	{
		putchar(c);
		buf[buf_idx] = c;
		buf_idx++;
	}
	else if (c == CHAR_BS)
	{
		buf_idx--;
		printf("\010 \010");
	}
	else if (c == CHAR_CR)
	{
		puts("");
		if (strlen(buf) > 0)
		{
			rc = tcli_cmd_handle(&buf[0]);
			puts(tcli_error(rc));
		}
		buf_idx = -1;
	}
	buf[buf_idx] = 0;
}
