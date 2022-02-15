#include <stdint.h>
#include "tty.h"
#include "tty_hw.h"

#define TX_BUF_SIZE     32
#define HW_RX_BUF_SIZE  32

#define VT100_CURSOR_BOL        "\033[999D"
#define VT100_CURSOR_EOL        "\033[999C"
#define VT100_CURSOR_UP         "\033[1A"
#define VT100_CURSOR_DOWN       "\033[1B"
#define VT100_CURSOR_RIGHT      "\033[1C"
#define VT100_CURSOR_LEFT       "\033[1D"
#define VT100_ERASE_EOS         "\033[0J"
#define VT100_ERASE_EOL         "\033[0K"
#define VT100_SAVE_CURSOR       "\0337"
#define VT100_RESTORE_CURSOR    "\0338"

#define ASCII_BS 	8
#define ASCII_LF  	10
#define ASCII_CR 	13
#define ASCII_ESC 	27
#define ASCII_DEL 	127

typedef enum
{
    STATE_CHAR,
    STATE_GOT_ESC,
    STATE_GOT_ESC_LB
} state_t;

static uint8_t tx_buf[TX_BUF_SIZE + 1];
static uint8_t hw_rx_buf[HW_RX_BUF_SIZE];
static int hw_rx_put_idx = 0;
static int hw_rx_get_idx = 0;

static void tty_puts(const void *str)
{
	int i;
	uint8_t *p = (uint8_t *)str;
	for (i = 0; p[i]; i++);
	tty_hw_tx(p, i);
}

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
    static int buf_end_idx = -1;
    static int cursor_idx = 0;
    int c;
    int rc = EOF;
    int i;

    if (buf_end_idx < 0)
    {
        buf_end_idx = 0;
        buf[buf_end_idx] = 0;
        tty_tx((uint8_t *)">", 1);
    }

    if ((c = tty_getkey()) != EOF)
    {
        switch (c)
        {
        case ASCII_BS:
            if (cursor_idx > 0)
            {
                buf_end_idx--;
                cursor_idx--;
                buf[buf_end_idx] = 0;
                for (i = cursor_idx; i < buf_end_idx; buf[i] = buf[i+1], i++);
                tty_puts(VT100_CURSOR_LEFT VT100_SAVE_CURSOR);
                tty_puts(&buf[cursor_idx]);
                tty_puts(VT100_ERASE_EOL VT100_RESTORE_CURSOR);
            }
            break;
        case ASCII_CR:
            rc = buf_end_idx;
            buf_end_idx = -1;
            cursor_idx = 0;
            tty_puts("\r\n");
            break;
        case KEY_LEFT:
            if (cursor_idx > 0)
            {
                cursor_idx--;
                tty_puts(VT100_CURSOR_LEFT);
            }
            break;
        case KEY_RIGHT:
            if (cursor_idx < buf_end_idx)
            {
                cursor_idx++;
                tty_puts(VT100_CURSOR_RIGHT);
            }
            break;
        default:
            if (buf_end_idx < (buf_len - 1) && c >= ' ' && c <= '~')
            {
                for (i = buf_end_idx; i > cursor_idx; buf[i] = buf[i-1], i--);
                buf_end_idx++;
                buf[buf_end_idx] = 0;
                buf[cursor_idx] = c;
                tty_puts(VT100_SAVE_CURSOR);
                tty_puts(&buf[cursor_idx]);
                tty_puts(VT100_RESTORE_CURSOR VT100_CURSOR_RIGHT);
                cursor_idx++;
            }
            break;
        }
    }

    return rc;
}
