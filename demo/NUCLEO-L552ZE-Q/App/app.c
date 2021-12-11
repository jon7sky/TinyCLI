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

#define CRSR_BOL	"\033[999D"
#define CRSR_EOL	"\033[999C"
#define CRSR_UP		"\033[%dA"
#define CRSR_DOWN	"\033[%dB"
#define CRSR_RIGHT	"\033[%dC"
#define CRSR_LEFT	"\033[%dD"
#define ERASE_TO_END_OF_SCREEN	"\033[0J"

typedef enum
{
	INIT,
	CHAR,
	GOT_ESC,
	GOT_ESC_LB
} state_t;

enum
{
	ASCII_BS = 8,
	ASCII_LF = 10,
	ASCII_CR = 13,
	ASCII_ESC = 27,
	ASCII_DEL = 127,
};

void app_init(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	puts("\nTinyCLI Demo\n");
}

#define BUF_CNT 	4
#define BUF_SIZE	256
static char buf[BUF_CNT][BUF_SIZE];

void app_run(void)
{
	static state_t state = INIT;
	static int buf_len = 0;
	static int buf_idx = 0;
	static int cur_buf_idx = 0;
	static int line_len = 25;
	char *b;
	int c;
	int i;

	b = &buf[cur_buf_idx][0];

	if (state != INIT && (c = getchar()) == EOF)
	{
		return;
	}

	if (state != INIT)
	{
		i = buf_idx / line_len;
		printf(CRSR_BOL);
		if (i) printf(CRSR_UP, i);
	}

	switch (state)
	{
	case INIT:
		b[0] = '>';
		b[1] = 0;
		buf_idx = buf_len = 1;
		state = CHAR;
		break;
	case CHAR:
		switch (c)
		{
		case ASCII_CR:
			printf(CRSR_BOL);
			i = buf_len / line_len + 1;
			printf(CRSR_DOWN "\n", i);
			printf("CMD: '%s'\n", b+1);
			state = INIT;
			return;
		case ASCII_ESC:
			state = GOT_ESC;
			break;
		case ASCII_BS:
			if (buf_idx > 1)
			{
				buf_idx--;
				buf_len--;
				for (i = buf_idx; i < buf_len; b[i] = b[i+1], i++);
			}
			break;
		default:
			if (c >= ' ' && c < CHAR_DEL && buf_len < (BUF_SIZE - 2))
			{
				for (i = buf_len; i > buf_idx; b[i] = b[i-1], i--);
				b[buf_idx++] = c;
			}	buf_len++;
			break;
		}
		break;
	case GOT_ESC:
		state = (c == '[' ? GOT_ESC_LB : CHAR);
		break;
	case GOT_ESC_LB:
		switch (c)
		{
		case 'A':
			break;
		case 'B':
			break;
		case 'C':
			if (buf_idx < buf_len)
			{
				buf_idx++;
			}
			break;
		case 'D':
			if (buf_idx > 1)
			{
				buf_idx--;
			}
			break;
		default:
			break;
		}
		state = CHAR;
	}
	b[buf_len] = 0;
	i = buf_len / line_len;
	printf("%s", b);
	if ((buf_len % line_len) == 0) printf(" ");
	printf(ERASE_TO_END_OF_SCREEN CRSR_BOL);
	if (i) printf(CRSR_UP, i);
	i = buf_idx % line_len;
	if (i) printf(CRSR_RIGHT, i);
	i = buf_idx / line_len;
	if (i) printf(CRSR_DOWN, i);

	return;
}

int tcli_cmd_handle_led(tcli_args_led_t *args)
{
	if (args->red || args->all)
	{
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, args->on);
	}
	if (args->green || args->all)
	{
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, args->on);
	}
	if (args->blue || args->all)
	{
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, args->on);
	}
	return TCLI_OK;
}

int tcli_cmd_handle_button_wait(tcli_args_button_wait_t *args)
{
	puts("Press the USER button. I'll wait.");
	while (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin) == GPIO_PIN_RESET);
	puts("Button pressed!");
	return TCLI_OK;
}

int tcli_cmd_handle_help(tcli_args_help_t *args)
{
	puts(TCLI_USAGE_TEXT);
	return TCLI_OK;
}
