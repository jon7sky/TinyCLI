#include <stdio.h>
#include "main.h"

UART_HandleTypeDef hlpuart1;

static uint8_t rxBuf[0x10];
static uint8_t rxPutIdx = 0;
static uint8_t rxGetIdx = 0;

void term_hw_init(void)
{
	HAL_UART_Receive_IT(&hlpuart1, &rxBuf[rxPutIdx], 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &hlpuart1)
	{
		//printf("got one! get = %d, put = %d\n", rxGetIdx, rxPutIdx);
		rxPutIdx = (rxPutIdx + 1) % sizeof(rxBuf);
		HAL_UART_Receive_IT(&hlpuart1, &rxBuf[rxPutIdx], 1);
	}
}

int _read(int file, char *ptr, int len)
{
	int lenRx = 0;

	while (len)
	{
		if (rxGetIdx == rxPutIdx)
		{
			break;
		}
		*ptr++ = rxBuf[rxGetIdx];
		rxGetIdx = (rxGetIdx + 1) % sizeof(rxBuf);
		lenRx++;
	}

	return lenRx ? lenRx : -1;
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
