#include <stdint.h>
#include "tty.h"
#include "tty_hw.h"

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
int rx_get_end_idx = 0;

#define RX_BUF_PUT(c) rx_buf[rx_put_idx] = c; rx_put_idx = (rx_put_idx + 1) % sizeof(rx_buf);

static tty_mode_t mode = TTY_MODE_CHAR;

int tty_tx(uint8_t *buf, uint32_t len)
{
    return tty_hw_tx(buf, len);
}

void tty_fill_rx_buf(uint8_t *buf, uint32_t len)
{
    static state_t state = STATE_CHAR;
    uint8_t c;

    while (len--)
    {
        c = *buf++;
        if (mode != TTY_MODE_CHAR)
        {
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
                state = c == '[' ? STATE_GOT_ESC_LB : STATE_CHAR;
                c = 0;
                break;
            case STATE_GOT_ESC_LB:
                switch (c)
                {
                case 'A':   c = KEY_UP;     break;
                case 'B':   c = KEY_DOWN;   break;
                case 'C':   c = KEY_RIGHT;  break;
                case 'D':   c = KEY_LEFT;   break;
                default:    c = 0;          break;
                }
                state = STATE_CHAR;
                break;
            }
        }
        if (c)
        {
            RX_BUF_PUT(c);
        }
    }
    return 0;
}
