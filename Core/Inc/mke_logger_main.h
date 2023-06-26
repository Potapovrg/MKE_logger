/*
 * mke_logger_main.h
 *
 *  Created on: 19 июн. 2023 г.
 *      Author: Roman
 */

#ifndef INC_MKE_LOGGER_MAIN_H_
#define INC_MKE_LOGGER_MAIN_H_

#include "main.h"
#include "usb_host.h"
#include "usbh_hid.h"

char Uart_Buf[100];

void mke_logger_main(void);

typedef struct
{
	uint8_t target;
	uint8_t button;
	int8_t mouse_x;
	int8_t mouse_y;
	int8_t wheel;
	uint8_t modifier;
	uint8_t reserved;
	uint8_t keycode1;
	uint8_t keycode2;
	uint8_t keycode3;
	uint8_t keycode4;
	uint8_t keycode5;
	uint8_t keycode6;
	uint8_t c_keycode1;
	uint8_t c_keycode2;
	uint8_t c_keycode3;
	uint8_t c_keycode4;
} bufferSPI;

#endif /* INC_MKE_LOGGER_MAIN_H_ */
