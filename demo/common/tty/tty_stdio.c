#include <stdint.h>

extern uint8_t rx_buf[0x100];
extern int rx_put_idx;
extern int rx_get_idx;

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
    tty_tx((uint8_t *) ptr, len);
    return len;
}
