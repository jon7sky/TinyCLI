#ifndef TTY_H_
#define TTY_H_

#define KEY_UP      128
#define KEY_DOWN    129
#define KEY_RIGHT   130
#define KEY_LEFT    131

typedef enum
{
    TTY_MODE_RAW,
    TTY_MODE_KEY,
    TTY_MODE_LINE
} tty_mode_t;

int tty_tx(uint8_t *buf, uint32_t len);
int tty_rx(uint8_t *buf, uint32_t len);

#endif /* TTY_H_ */
