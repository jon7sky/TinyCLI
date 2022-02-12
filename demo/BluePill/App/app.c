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

int tcli_cmd_handle_led(tcli_args_led_t *args)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, (args->on ? 0 : 1));
	return TCLI_OK;
}


int tcli_cmd_handle_bambi(tcli_args_bambi_t *args)
{
    extern const unsigned char bambi[];
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
