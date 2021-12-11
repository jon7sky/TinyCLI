#include <stdio.h>
#include "main.h"

UART_HandleTypeDef hlpuart1;

int getchar(void)
{
    HAL_StatusTypeDef hstatus;
    uint8_t b;
    hstatus = HAL_UART_Receive(&hlpuart1, &b, 1, 0);
    return (hstatus != HAL_OK ? EOF : b);
}

int _write(int file, char *ptr, int len) {
	int i;
	uint8_t b;

    for (i = 0; i < len; i++)
    {
    	b = (uint8_t)*ptr++;
    	HAL_UART_Transmit(&hlpuart1, &b, 1, HAL_MAX_DELAY);
    	if (b == '\n')
    	{
    		b = '\r';
        	HAL_UART_Transmit(&hlpuart1, &b, 1, HAL_MAX_DELAY);
    	}
    }
	return len;
}
