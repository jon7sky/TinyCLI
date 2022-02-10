#include "main.h"
#include "tty_hw.h"
#include "tty.h"
#include "usbd_def.h"

uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

uint8_t tty_hw_tx(uint8_t *buf, uint32_t len)
{
    uint8_t rc;
    while ((rc = CDC_Transmit_FS(buf, len)) == USBD_BUSY);
    return rc;
}

#if 0
int tty_rx_hw(uint8_t *buf, uint32_t len)
{
    int len_rx;
    while (len && rx_get_idx != rx_put_idx)
    {
        *buf++ = rx_buf[rx_get_idx];
        len--;
        len_rx++;
        rx_get_idx = (rx_get_idx + 1) % sizeof(rx_buf);
    }
    return len_rx ? len_rx : -1;
}
#endif

void tty_hw_fill_rx_buf(uint8_t *buf, uint32_t len)
{
    tty_fill_rx_buf(buf, len);
}
