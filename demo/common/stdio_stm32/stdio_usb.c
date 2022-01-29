#include <stdio.h>
#include "main.h"
#include "usbd_def.h"

uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

static uint8_t rx_buf[0x10];
static int rx_put_idx = 0;
static int rx_get_idx = 0;

void stdio_usb_init(void)
{
}

void stdio_put_rx_buf(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        rx_buf[rx_put_idx] = *buf++;
        rx_put_idx = (rx_put_idx + 1) % sizeof(rx_buf);
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
        while (CDC_Transmit_FS(&b, 1) == USBD_BUSY);
        if (b == '\n')
        {
            b = '\r';
            while (CDC_Transmit_FS(&b, 1) == USBD_BUSY);
        }
    }
    return len;
}
