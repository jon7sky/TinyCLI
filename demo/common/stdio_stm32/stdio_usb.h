/*
 * stdio_usb.h
 *
 *  Created on: Jan 28, 2022
 *      Author: john
 */

#ifndef STDIO_USB_H_
#define STDIO_USB_H_

void stdio_usb_init(void);
void stdio_put_rx_buf(uint8_t *buf, uint32_t len);

#endif /* STDIO_USB_H_ */
