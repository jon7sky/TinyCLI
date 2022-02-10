#include <stdint.h>
#include "tty.h"
#include "tty_hw.h"

#define MAX_LINE_SIZE 128

typedef enum
{
    STATE_INIT,
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

uint8_t rx_buf[0x100];
int rx_put_idx = 0;
int rx_get_idx = 0;

#define RX_BUF_PUT(c) rx_buf[rx_put_idx] = c; rx_put_idx = (rx_put_idx + 1) % sizeof(rx_buf);

static tty_mode_t mode = TTY_MODE_RAW;

int tty_tx(uint8_t *buf, uint32_t len)
{
    return tty_hw_tx(buf, len);
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

        // We're in line mode.
        swtich (c)
        {
        case ASCII_BS:
            if (buf_idx > 0)
            {
                buf_idx--;
            }
            break;
        case ASCII_CR:
            rx_put_idx = (rx_put_idx + buf_idx) % sizeof(rx_buf);
            buf_idx = 0;
            break;
        default:
            if (buf_idx < MAX_LINE_SIZE && c >= ' ' && c <= '~')
            {
                rx_buf[(rx_put_idx + buf_idx) % sizeof(rx_buf)] = c;
                buf_idx++;
            }
            break;
        }
    }

    return;
}
