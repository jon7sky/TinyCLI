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
	puts("\nTinyCLI Demo\n");
}

static char buf[256];

void app_run(void)
{
	int c;
	int i;
	int rc;
	static int buf_idx = 0;
	static int buf_len = 0;

	if (buf_idx == 0)
	{
		buf[0] = '>';
		buf[1] = 0;
		buf_idx = buf_len = 1;
		printf(buf);
	}

	if ((c = getchar()) == EOF)
	{
		return;
	}

	switch (c)
	{
	case CHAR_CR:
		printf("\n");
		rc = tcli_cmd_handle(&buf[1]);
		puts(tcli_error(rc));
		buf_len = buf_idx = 0;
		return;
	case CHAR_BS:
		if (buf_idx > 1)
		{
			buf_idx--;
			buf_len--;
			for (i = buf_idx; i < buf_len; buf[i] = buf[i+1], i++);
		}
		break;
	case CHAR_LEFT:
		if (buf_idx > 1)
		{
			buf_idx--;
		}
		break;
	case CHAR_RIGHT:
		if (buf_idx < buf_len)
		{
			buf_idx++;
		}
		break;
	default:
		if (c >= ' ' && c < CHAR_DEL && buf_len < (sizeof(buf) - 2))
		{
			for (i = buf_len; i > buf_idx; buf[i] = buf[i-1], i--);
			buf[buf_idx++] = c;
		}	buf_len++;
		break;
	}
	buf[buf_len] = 0;
	printf("\033[999D%s\033[K\033[999D\033[%dC", buf, buf_idx);
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
