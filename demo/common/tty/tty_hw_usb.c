#include "main.h"
#include "tty_hw.h"
#include "tty.h"
#include "usbd_def.h"

uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

void tty_hw_tx(uint8_t *buf, uint32_t len)
{
    uint8_t rc;
    while ((rc = CDC_Transmit_FS(buf, len)) == USBD_BUSY);
}

void tty_hw_fill_rx_buf(uint8_t *buf, uint32_t len)
{
    tty_fill_rx_buf(buf, len);
}
