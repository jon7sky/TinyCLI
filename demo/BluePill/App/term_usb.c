#include <stdio.h>
#include "main.h"
#include "usbd_def.h"

uint8_t input_buf[256];
int put_idx = 0;
int get_idx = 0;

void term_add_buf(uint8_t *buf, uint32_t len)
{
    while (len)
    {
        input_buf[put_idx] = *buf++;
        len--;
        put_idx = (put_idx + 1) % sizeof(input_buf);
    }
}

int getchar(void)
{
    if (get_idx != put_idx)
    {
        uint8_t b;
        b = input_buf[get_idx];
        get_idx = (get_idx + 1) % sizeof(input_buf);
        return b;
    }
    return EOF;
}

int _write(int file, char *ptr, int len) {
    int i;
    uint8_t b;

    for (i = 0; i < len; i++)
    {
        b = (uint8_t)*ptr++;
        while (CDC_Transmit_FS(&b, 1) == USBD_BUSY);
        if (b == '\n')
        {
            b = '\r';
            while (CDC_Transmit_FS(&b, 1) == USBD_BUSY);
        }
    }
    return len;
}
