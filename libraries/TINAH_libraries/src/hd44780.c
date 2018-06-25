/*
 * Copyright (c) 2018 Amar Shah
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "hd44780.h"
#include "hd44780_config.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define HD44780_DELAY(MICROSECONDS) _delay_us(MICROSECONDS)

#define HD44780_COMMAND_DELAY() HD44780_DELAY(40)

#define SET_BIT(VAR, INDEX) (VAR) |= (1 << (INDEX))
#define CLR_BIT(VAR, INDEX) (VAR) &= ~(1 << (INDEX))

void hd44780_send_command(uint8_t command);
void hd44780_send_byte(uint8_t command);

void hd44780_init(uint8_t mode) {
	SET_BIT(HD44780_RS_DIR, HD44780_RS_PIN);
	SET_BIT(HD44780_RW_DIR, HD44780_RW_PIN);
	SET_BIT(HD44780_EN_DIR, HD44780_EN_PIN);

	HD44780_DATA_DIR = 0x00;
	HD44780_DATA_PORT = 0xFF;
	SET_BIT(HD44780_RW_PORT, HD44780_RW_PIN);
	CLR_BIT(HD44780_RS_PORT, HD44780_RS_PIN);
	SET_BIT(HD44780_EN_PORT, HD44780_EN_PIN);

	uint8_t init_command = HD44780_MODE_COMMAND | HD44780_MODE_8_BIT | mode;
	hd44780_send_command(init_command);
	HD44780_DELAY(4500);
	hd44780_send_command(init_command);
	HD44780_DELAY(200);
	hd44780_send_command(init_command);
	HD44780_DELAY(50);
	hd44780_send_command(init_command);
	HD44780_DELAY(50);

	hd44780_send_command(HD44780_CLEAR_COMMAND);
	HD44780_DELAY(2000);
}

void hd44780_send_command(uint8_t command) {
	CLR_BIT(HD44780_RS_PORT, HD44780_RS_PIN);
	hd44780_send_byte(command);
}

void hd44780_send_data(uint8_t data) {
	SET_BIT(HD44780_RS_PORT, HD44780_RS_PIN);
	hd44780_send_byte(data);
}

bool hd44780_is_busy() {
	return (HD44780_DATA_PORT_IN & HD44780_BUSYFLAG) ? true : false;
}

void hd44780_block_until_ready() {
	while (hd44780_is_busy()) {
		continue;
	}
}

void hd44780_send_byte(uint8_t value) {
	CLR_BIT(HD44780_RW_PORT, HD44780_RW_PIN);
	HD44780_DATA_PORT = value;
	HD44780_DATA_DIR = 0xFF;

	CLR_BIT(HD44780_EN_PORT, HD44780_EN_PIN);
	HD44780_DELAY(1);
	SET_BIT(HD44780_EN_PORT, HD44780_EN_PIN);
	HD44780_DELAY(1);
	CLR_BIT(HD44780_EN_PORT, HD44780_EN_PIN);
	HD44780_DELAY(1);


	HD44780_DATA_DIR = 0x00;
	HD44780_DATA_PORT = 0xFF;
	SET_BIT(HD44780_RW_PORT, HD44780_RW_PIN);
	CLR_BIT(HD44780_RS_PORT, HD44780_RS_PIN);
	SET_BIT(HD44780_EN_PORT, HD44780_EN_PIN);
}