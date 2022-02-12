#include <stdio.h>
#include "main.h"
#include "tcli.h"

void app_init(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	puts("\n\nTinyCLI Demo");
}

void app_run(void)
{
    int rc;
    char buf[128];
    static int print_prompt = 1;

    if (print_prompt)
    {
        putchar('>');
        print_prompt = 0;
    }

    if (fgets((char *)&buf[0], sizeof(buf), stdin) == NULL)
    {
        return;
    }

    if (buf[0])
    {
        rc = tcli_cmd_handle(buf);
        puts(tcli_error(rc));
    }

    print_prompt = 1;
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
	puts("Press the blue USER button. I'll wait.");
	while (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin) == GPIO_PIN_RESET);
	puts("Button pressed!");
	return TCLI_OK;
}

int tcli_cmd_handle_help(tcli_args_help_t *args)
{
	puts(TCLI_USAGE_TEXT);
	return TCLI_OK;
}
