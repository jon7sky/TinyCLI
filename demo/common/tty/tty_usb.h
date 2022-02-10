/*
 * tty_usb.h
 *
 *  Created on: Jan 30, 2022
 *      Author: john
 */

#ifndef TTY_USB_H_
#define TTY_USB_H_

uint8_t tty_tx(uint8_t *buf, uint32_t len);
int tty_rx(uint8_t *buf, uint32_t len);



#endif /* TTY_USB_H_ */
