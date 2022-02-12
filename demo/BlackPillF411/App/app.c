#include <stdio.h>
#include "term.h"
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

int tcli_cmd_handle_led(tcli_args_led_t *args)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, args->on ? 0 : 1);
	return TCLI_OK;
}

int tcli_cmd_handle_button_wait(tcli_args_button_wait_t *args)
{
	puts("Press the KEY button. I'll wait.");
	while (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET);
	puts("Button pressed!");
	return TCLI_OK;
}

int tcli_cmd_handle_help(tcli_args_help_t *args)
{
	puts(TCLI_USAGE_TEXT);
	return TCLI_OK;
}
