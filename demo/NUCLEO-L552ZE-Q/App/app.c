/*
 * app.c
 *
 *  Created on: Dec 5, 2021
 *      Author: john
 */

#include <stdio.h>
#include "main.h"
#include "vt100.h"

void app_init(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	puts("Hello world");
}

static char buf[256];

void app_run(void)
{
	int c;
	static int buf_idx = -1;

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
		buf_idx = -1;
	}
	buf[buf_idx] = 0;
}
