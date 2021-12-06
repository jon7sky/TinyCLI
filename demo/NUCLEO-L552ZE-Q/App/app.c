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
