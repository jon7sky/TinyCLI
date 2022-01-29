#include <stdio.h>
#include "main.h"
#include "usbd_def.h"

uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

static uint8_t rx_buf[0x100];
static int rx_put_idx = 0;
static int rx_get_idx = 0;

void stdio_usb_init(void)
{
}

void term_add_buf(uint8_t *buf, uint32_t len)
{
    while (len)
    {
        rx_buf[rx_put_idx] = *buf++;
        len--;
        rx_put_idx = (rx_put_idx + 1) % sizeof(rx_buf);
    }
}

int getchar(void)
{
    if (rx_get_idx != rx_put_idx)
    {
        uint8_t b;
        b = rx_buf[rx_get_idx];
        rx_get_idx = (rx_get_idx + 1) % sizeof(rx_buf);
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
