#include <stdio.h>
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

#define ACCUM_SIZE	2
#define BUF_CNT 	4
#define BUF_SIZE	256
static char buf[BUF_CNT][BUF_SIZE];

void term_init(void)
{

}

void term_run(void)
{
	static state_t state = STATE_INIT;
	static int buf_len = 0;
	static int buf_idx = 0;
	static int cur_buf_idx = 0;
	static int line_len = 80;
	static int accum[ACCUM_SIZE];
	static int accum_idx;
	char *b;
	int c;
	int i;

	b = &buf[cur_buf_idx][0];

	if (state != STATE_INIT && (c = getchar()) == EOF)
	{
		return;
	}

	switch (state)
	{
	case STATE_INIT:
		b[0] = '>';
		b[1] = 0;
		buf_idx = buf_len = 1;
		state = STATE_CHAR;
		printf(VT100_CURSOR_EOL VT100_ACTIVE_POS_REPORT VT100_CURSOR_BOL VT100_SAVE_CURSOR ">");
		return;
	case STATE_CHAR:
		switch (c)
		{
		case ASCII_CR:
			printf(VT100_CURSOR_BOL);
			i = buf_len / line_len + 1;
			printf(VT100_CURSOR_DOWN "\n", i);
			term_cmd_exe(b+1);
			state = STATE_INIT;
			return;
		case ASCII_ESC:
			state = STATE_GOT_ESC;
			return;
		case ASCII_BS:
			if (buf_idx > 1)
			{
				buf_idx--;
				buf_len--;
				for (i = buf_idx; i < buf_len; b[i] = b[i+1], i++);
			}
			break;
		default:
			if (c >= ' ' && c < ASCII_DEL && buf_len < (BUF_SIZE - 2))
			{
				for (i = buf_len; i > buf_idx; b[i] = b[i-1], i--);
				b[buf_idx++] = c;
			}	buf_len++;
			break;
		}
		break;
	case STATE_GOT_ESC:
		accum_idx = 0;
		accum[0] = accum[1] = 0;
		state = (c == '[' ? STATE_GOT_ESC_LB : STATE_CHAR);
		return;
	case STATE_GOT_ESC_LB:
		if (c >= '0' && c <= '9')
		{
			accum[accum_idx] = accum[accum_idx] * 10 + (c & 0xf);
			return;
		}
		switch (c)
		{
		case ';':
			if (accum_idx < ACCUM_SIZE)
			{
				accum_idx++;
			}
			return;
		case 'R': // Cursor position report
			line_len = accum[1];
			state = STATE_CHAR;
			return;
		case 'A': // Cursor up
		case 'B': // Cursor down
			state = STATE_CHAR;
			return;
		case 'C': // Cursor right
		case 'D': // Cursor left
			if (c == 'C' && buf_idx < buf_len)
			{
				buf_idx++;
			}
			if (c == 'D' && buf_idx > 1)
			{
				buf_idx--;
			}
			printf(VT100_RESTORE_CURSOR);
			if ((i = buf_idx % line_len) > 0) printf(VT100_CURSOR_RIGHT, i);
			if ((i = buf_idx / line_len) > 0) printf(VT100_CURSOR_DOWN, i);
			state = STATE_CHAR;
			return;
		default:
			state = STATE_CHAR;
			return;
		}
	}

	// If we got here, the buffer contents have changed in some way.
	b[buf_len] = 0;
	printf(VT100_RESTORE_CURSOR "%s \010" VT100_CURSOR_BOL, b);
	if ((i = buf_len / line_len) > 0) printf(VT100_CURSOR_UP, i);
	// Cursor should now be sitting right at the '>' prompt.
	// Save the position and then move it to where it's supposed to be.
	printf(VT100_SAVE_CURSOR);
	if ((i = buf_idx % line_len) > 0) printf(VT100_CURSOR_RIGHT, i);
	if ((i = buf_idx / line_len) > 0) printf(VT100_CURSOR_DOWN, i);

	return;
}

__attribute__((weak)) void term_cmd_exe(char *buf)
{
	printf("Exe Cmd: '%s'\n", buf);
}
