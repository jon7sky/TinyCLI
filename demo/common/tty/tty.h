#ifndef TTY_H_
#define TTY_H_

#include <stdio.h>

#define KEY_UP      128
#define KEY_DOWN    129
#define KEY_RIGHT   130
#define KEY_LEFT    131

int tty_tx(uint8_t *buf, uint32_t len);
void tty_fill_rx_buf(uint8_t *buf, uint32_t len);
int tty_getchar(void);
int tty_getkey(void);
int tty_getline(char *buf, int buf_len);

#endif /* TTY_H_ */
