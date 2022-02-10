/*
 * tty_hw.h
 *
 *  Created on: Feb 7, 2022
 *      Author: john
 */

#ifndef TTY_HW_H_
#define TTY_HW_H_

uint8_t tty_hw_tx(uint8_t *buf, uint32_t len);
void tty_hw_fill_rx_buf(uint8_t *buf, uint32_t len);

#endif /* TTY_HW_H_ */
