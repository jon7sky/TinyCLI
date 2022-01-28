#include <stdio.h>
#include <string.h>
#include "term.h"

#define VT100_CURSOR_BOL		"\033[999D"
#define VT100_CURSOR_EOL		"\033[999C"
#define VT100_CURSOR_UP			"\033[%dA"
#define VT100_CURSOR_DOWN		"\033[%dB"
#define VT100_CURSOR_RIGHT		"\033[%dC"
#define VT100_CURSOR_LEFT		"\033[%dD"
#define VT100_ERASE_EOS			"\033[0J"
#define VT100_SAVE_CURSOR		"\0337"
#define VT100_RESTORE_CURSOR	"\0338"
#define VT100_ACTIVE_POS_REPORT "\033[6n"

typedef enum
{
	STATE_INIT,
	STATE_CHAR,
	STATE_GOT_ESC,
	STATE_GOT_ESC_LB
} state_t;

enum
{
	ASCII_BS = 8,
	ASCII_LF = 10,
	ASCII_CR = 13,
	ASCII_ESC = 27,
	ASCII_DEL = 127,
};

#define BUF_SIZE	256
static char b[BUF_SIZE] = {0, };

void term_init(void)
{
	term_hw_init();
}

void term_run(void)
{
	static state_t state = STATE_INIT;
	static int buf_len = 0;
	int c;

	if (state != STATE_INIT && (c = getchar()) == EOF)
	{
		return;
	}

	switch (state)
	{
	case STATE_INIT:
		b[0] = '>';
		b[1] = 0;
		buf_len = 1;
		state = STATE_CHAR;
		printf(b);
		return;
	case STATE_CHAR:
		switch (c)
		{
		case ASCII_CR:
			printf("\n");
			term_cmd_exe(b+1);
			state = STATE_INIT;
			return;
		case ASCII_ESC:
			state = STATE_GOT_ESC;
			return;
		case ASCII_BS:
			if (buf_len > 1)
			{
				buf_len--;
				printf("\010 \010");
			}
			break;
		default:
			if (c >= ' ' && c < ASCII_DEL && buf_len < (BUF_SIZE - 2))
			{
				b[buf_len++] = c;
				printf("%c", c);
			}
			break;
		}
		break;
	case STATE_GOT_ESC:
		state = (c == '[' ? STATE_GOT_ESC_LB : STATE_CHAR);
		return;
	case STATE_GOT_ESC_LB:
		if (c >= 'A' && c <= 'z')
		{
			state = STATE_CHAR;
		}
		break;
	}
	b[buf_len] = 0;
	return;
}

__attribute__((weak)) void term_cmd_exe(char *buf)
{
	printf("Exe Cmd: '%s'\n", buf);
}
