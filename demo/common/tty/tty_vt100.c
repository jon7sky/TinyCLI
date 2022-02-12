#include <stdint.h>
#include "tty.h"
#include "tty_hw.h"

#define TX_BUF_SIZE     32
#define RX_BUF_SIZE     128

typedef enum
{
    STATE_CHAR,
    STATE_GOT_ESC,
    STATE_GOT_ESC_LB
} state_t;

enum
{
    ASCII_BS = 8,
    ASCII_LF = 10,
    ASCII_CR = 13,
    ASCII_ESC = 27,
    ASCII_DEL = 127,
};

uint8_t tx_buf[TX_BUF_SIZE + 1];
uint8_t rx_buf[RX_BUF_SIZE];
int rx_put_idx = 0;
int rx_get_idx = 0;

#define RX_BUF_PUT(c) if (rx_put_idx < (sizeof(rx_buf) - 1)) rx_buf[rx_put_idx++] = c;

static tty_mode_t mode = TTY_MODE_LINE;

int tty_tx(uint8_t *buf, uint32_t len)
{
    int i = 0;
    int bytes_left = len;

    while (bytes_left--)
    {
        if ((tx_buf[i++] = *buf++) == '\n')
        {
            tx_buf[i++] = '\r';
        }
        if (bytes_left == 0 || i >= TX_BUF_SIZE)
        {
            tty_hw_tx(&tx_buf[0], i);
            i = 0;
        }
    }
    return len;
}

int tty_rx(uint8_t *ptr, uint32_t len)
{
    int len_rx;

    for (len_rx = 0; len_rx < len; len_rx++)
    {
        if (rx_get_idx >= rx_put_idx)
        {
            rx_get_idx = 0;
            rx_put_idx = 0;
            break;
        }
        *ptr++ = rx_buf[rx_get_idx++];
    }

    return len_rx ? len_rx : -1;
}

void tty_fill_rx_buf(uint8_t *buf, uint32_t len)
{
    static state_t state = STATE_CHAR;
    uint8_t c;
    static int buf_idx = 0;

    while (len--)
    {
        c = *buf++;

        // If raw mode, just insert the character into the buffer as-is.
        if (mode == TTY_MODE_RAW)
        {
            RX_BUF_PUT(c);
            continue;
        }

        // Convert ESC-[ sequences into special key codes.
        switch (state)
        {
        case STATE_CHAR:
            if (c == ASCII_ESC)
            {
                state = STATE_GOT_ESC;
                c = 0;
            }
            break;
        case STATE_GOT_ESC:
            state = (c == '[' ? STATE_GOT_ESC_LB : STATE_CHAR);
            c = 0;
            break;
        case STATE_GOT_ESC_LB:
            c = (c >= 'A' && c <= 'D') ? c - 'A' + KEY_UP : 0;
            state = STATE_CHAR;
            break;
        }
        if (c == 0)
        {
            continue;
        }

        // If key mode, insert char or key code into buffer.
        if (mode == TTY_MODE_KEY)
        {
            RX_BUF_PUT(c);
            continue;
        }

        // We're in line mode. If the last line wasn't taken yet, then throw away these characters.
        if (rx_put_idx > rx_get_idx)
        {
            break;
        }
        switch (c)
        {
        case ASCII_BS:
            if (buf_idx > 0)
            {
                buf_idx--;
                tty_tx("\010 \010", 3);
            }
            break;
        case ASCII_CR:
            rx_buf[buf_idx++] = 0;
            rx_put_idx = buf_idx;
            buf_idx = 0;
            tty_tx("\r\n", 2);
            break;
        default:
            if (buf_idx < (sizeof(rx_buf) - 1) && c >= ' ' && c <= '~')
            {
                rx_buf[buf_idx++] = c;
                tty_tx(&c, sizeof(c));
            }
            break;
        }
    }

    return;
}
