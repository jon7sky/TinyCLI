#include "tty_hw_uart.h"
#include "tty.h"

static UART_HandleTypeDef *stdio_uart;

uint8_t byte;

void tty_hw_uart_init(UART_HandleTypeDef *uartp)
{
    stdio_uart = uartp;
    HAL_UART_Receive_IT(stdio_uart, &byte, 1);
}

uint8_t tty_hw_tx(uint8_t *buf, uint32_t len)
{
    HAL_UART_Transmit(stdio_uart, buf, len, HAL_MAX_DELAY);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == stdio_uart)
    {
        tty_fill_rx_buf(&byte, 1);
        HAL_UART_Receive_IT(stdio_uart, &byte, 1);
    }
}
