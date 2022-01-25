#include <stdio.h>
#include "term.h"
#include "main.h"
#include "tcli.h"

void app_init(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	puts("\nTinyCLI Demo\n");
}

void app_run(void)
{
}

void term_cmd_exe(char *buf)
{
	int rc;
	static char tcli_buf[256];

	if (*buf == 0)
	{
		return;
	}
	strncpy(tcli_buf, buf, sizeof(tcli_buf));
	rc = tcli_cmd_handle(tcli_buf);
	puts(tcli_error(rc));
}

int tcli_cmd_handle_led(tcli_args_led_t *args)
{
	if (args->red || args->green || args->blue || args->all)
	{
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, (args->on ? 0 : 1));
	}
	return TCLI_OK;
}

int tcli_cmd_handle_button_wait(tcli_args_button_wait_t *args)
{
	puts("What button?");
	// while (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin) == GPIO_PIN_RESET);
	// puts("Button pressed!");
	return TCLI_OK;
}

int tcli_cmd_handle_help(tcli_args_help_t *args)
{
	puts(TCLI_USAGE_TEXT);
	return TCLI_OK;
}
