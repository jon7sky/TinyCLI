/*
 * vt100.c
 *
 *  Created on: Dec 5, 2021
 *      Author: john
 */

#include "main.h"
#include "vt100.h"
#include <stdio.h>

UART_HandleTypeDef hlpuart1;

typedef enum
{
	READ_STATE_PASSTHROUGH,
	READ_STATE_GOT_ESC,
	READ_STATE_GOT_ESC_LB
} read_state_t;

int getchar(void)
{
	static read_state_t state = READ_STATE_PASSTHROUGH;

    HAL_StatusTypeDef hstatus;
    uint8_t b;
    hstatus = HAL_UART_Receive(&hlpuart1, &b, 1, 0);
    if (hstatus != HAL_OK)
    {
    	return EOF;
    }
    switch (state)
    {
    case READ_STATE_PASSTHROUGH:
    	if (b == 27)
    	{
    		state = READ_STATE_GOT_ESC;
    		break;
    	}
		return b;
    case READ_STATE_GOT_ESC:
    	state = (b == '[' ? READ_STATE_GOT_ESC_LB : READ_STATE_PASSTHROUGH);
		break;

    case READ_STATE_GOT_ESC_LB:
    	state = READ_STATE_PASSTHROUGH;
    	switch (b)
    	{
    	case 'A':	return CHAR_UP;
    	case 'B':	return CHAR_DOWN;
    	case 'C':	return CHAR_LEFT;
    	case 'D':	return CHAR_RIGHT;
    	default:	break;
    	}
    }
    return EOF;
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
