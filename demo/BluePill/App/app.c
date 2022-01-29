#include <stdio.h>
#include "term.h"
#include "main.h"
#include "tcli.h"

extern const unsigned char bambi[];

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
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, (args->on ? 0 : 1));
	return TCLI_OK;
}


int tcli_cmd_handle_bambi(tcli_args_bambi_t *args)
{
    const unsigned char *p;
    uint32_t new_tick = 0;
    uint32_t last_tick = 0;

    for (p = &bambi[0]; *p; p++)
    {
        putchar(*p);
        while ((new_tick = HAL_GetTick()) == last_tick);
        last_tick = new_tick;
    }

    // Reset scrolling
    printf("\033[r");
	return TCLI_OK;
}

int tcli_cmd_handle_help(tcli_args_help_t *args)
{
	puts(TCLI_USAGE_TEXT);
	return TCLI_OK;
}
