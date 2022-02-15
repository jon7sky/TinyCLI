#include <stdint.h>
#include "tty.h"
#include "tty_hw.h"

#define TX_BUF_SIZE     32
#define HW_RX_BUF_SIZE  32

#define VT100_CURSOR_BOL        "\033[999D"
#define VT100_CURSOR_EOL        "\033[999C"
#define VT100_CURSOR_UP         "\033[%dA"
#define VT100_CURSOR_DOWN       "\033[%dB"
#define VT100_CURSOR_RIGHT      "\033[%dC"
#define VT100_CURSOR_LEFT       "\033[%dD"
#define VT100_ERASE_EOS         "\033[0J"
#define VT100_ERASE_EOL         "\033[0K"
#define VT100_SAVE_CURSOR       "\0337"
#define VT100_RESTORE_CURSOR    "\0338"

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

static uint8_t tx_buf[TX_BUF_SIZE + 1];
static uint8_t hw_rx_buf[HW_RX_BUF_SIZE];
static int hw_rx_put_idx = 0;
static int hw_rx_get_idx = 0;

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

void tty_fill_rx_buf(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        hw_rx_buf[hw_rx_put_idx] = *buf++;
        hw_rx_put_idx = (hw_rx_put_idx + 1) % sizeof(hw_rx_buf);
    }
}

int tty_getchar(void)
{
    int c = EOF;

    if (hw_rx_get_idx != hw_rx_put_idx)
    {
        c = hw_rx_buf[hw_rx_get_idx];
        hw_rx_get_idx = (hw_rx_get_idx + 1) % sizeof(hw_rx_buf);
    }
    return c;
}

int tty_getkey(void)
{
    static state_t state = STATE_CHAR;
    int c;

    if ((c = tty_getchar()) != EOF)
    {
        switch (state)
        {
        case STATE_CHAR:
            if (c == ASCII_ESC)
            {
                state = STATE_GOT_ESC;
                c = EOF;
            }
            break;
        case STATE_GOT_ESC:
            state = (c == '[' ? STATE_GOT_ESC_LB : STATE_CHAR);
            c = EOF;
            break;
        case STATE_GOT_ESC_LB:
            c = (c >= 'A' && c <= 'D') ? c - 'A' + KEY_UP : EOF;
            state = STATE_CHAR;
            break;
        }
    }
    return c;
}

int tty_getline(char *buf, int buf_len)
{
    static int buf_idx = -1;
    static int cursor_idx = 0;
    int c;
    int rc = EOF;
    int i;

    if (buf_idx < 0)
    {
        buf_idx = 0;
        tty_tx((uint8_t *)">", 1);
    }

    if ((c = tty_getkey()) != EOF)
    {
        switch (c)
        {
        case ASCII_BS:
            if (cursor_idx > 0)
            {
                buf_idx--;
                cursor_idx--;
                for (i = cursor_idx; i < buf_idx; i++)
                {
                    buf[i] = buf[i+1];
                }
                tty_tx("\033[D\0337", 5);
                tty_tx(&buf[cursor_idx], buf_idx - cursor_idx);
                tty_tx(" \0338", 3);
            }
            break;
        case ASCII_CR:
            buf[buf_idx++] = 0;
            rc = buf_idx;
            buf_idx = -1;
            cursor_idx = 0;
            tty_tx((uint8_t *)"\r\n", 2);
            break;
        case KEY_LEFT:
            if (cursor_idx > 0)
            {
                cursor_idx--;
                tty_tx("\033[D", 3);
            }
            break;
        case KEY_RIGHT:
            if (cursor_idx < buf_idx)
            {
                cursor_idx++;
                tty_tx("\033[C", 3);
            }
            break;
        default:
            if (buf_idx < (buf_len - 1) && c >= ' ' && c <= '~')
            {
                for (i = buf_idx; i > cursor_idx; i--)
                {
                    buf[i] = buf[i-1];
                }
                buf[cursor_idx] = c;
                buf_idx++;
                tty_tx("\0337", 2);
                tty_tx((uint8_t *)&buf[cursor_idx], buf_idx - cursor_idx);
                tty_tx("\0338\033[C", 5);
                cursor_idx++;
            }
            break;
        }
    }

    return rc;
}
