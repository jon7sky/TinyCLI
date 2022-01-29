#include <stdio.h>
#include "main.h"
#include "stdio_uart.h"

static UART_HandleTypeDef *stdio_uart;

static uint8_t rx_buf[0x10];
static uint8_t rx_put_idx = 0;
static uint8_t rx_get_idx = 0;

void stdio_uart_init(UART_HandleTypeDef *uartp)
{
    stdio_uart = uartp;
	HAL_UART_Receive_IT(stdio_uart, &rx_buf[rx_put_idx], 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == stdio_uart)
	{
		rx_put_idx = (rx_put_idx + 1) % sizeof(rx_buf);
		HAL_UART_Receive_IT(stdio_uart, &rx_buf[rx_put_idx], 1);
	}
}

int _read(int file, char *ptr, int len)
{
    int len_rx;

    for (len_rx = 0; len_rx < len; len_rx++)
    {
        if (rx_get_idx == rx_put_idx)
        {
            break;
        }
        *ptr++ = rx_buf[rx_get_idx];
        rx_get_idx = (rx_get_idx + 1) % sizeof(rx_buf);
    }

    return len_rx ? len_rx : -1;
}

int _write(int file, char *ptr, int len) {
	int i;
	uint8_t b;

    for (i = 0; i < len; i++)
    {
    	b = (uint8_t)*ptr++;
    	HAL_UART_Transmit(stdio_uart, &b, 1, HAL_MAX_DELAY);
    	if (b == '\n')
    	{
    		b = '\r';
        	HAL_UART_Transmit(stdio_uart, &b, 1, HAL_MAX_DELAY);
    	}
    }
	return len;
}
