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

#ifndef HD44780_H
#define HD44780_H

#include <stdint.h>
#include <stdbool.h>

#define HD44780_CLEAR_COMMAND 0x01

#define HD44780_HOME_COMMAND 0x02

#define HD44780_ENTRY_COMMAND 0x04
#define HD44780_ENTRY_RIGHT 0x00
#define HD44780_ENTRY_LEFT 0x02
#define HD44780_ENTRY_INCREMENT 0x01
#define HD44780_ENTRY_DECREMENT 0x00

#define HD44780_DISP_CTRL_COMMAND 0x08
#define HD44780_DISP_CTRL_DISPLAY_ON 0x04
#define HD44780_DISP_CTRL_DISPLAY_OFF 0x00
#define HD44780_DISP_CTRL_CURSOR_ON 0x02
#define HD44780_DISP_CTRL_CURSOR_OFF 0x00
#define HD44780_DISP_CTRL_BLINK_ON 0x01
#define HD44780_DISP_CTRL_BLINK_OFF 0x00

#define HD44780_SHIFT_COMMAND 0x10
#define HD44780_SHIFT_DISPLAY 0x08
#define HD44780_SHIFT_CURSOR 0x00
#define HD44780_SHIFT_RIGHT 0x04
#define HD44780_SHIFT_LEFT 0x00

#define HD44780_MODE_COMMAND 0x20
#define HD44780_MODE_8_BIT 0x10
#define HD44780_MODE_4_BIT 0x00
#define HD44780_MODE_2_LINE 0x08
#define HD44780_MODE_1_LINE 0x00
#define HD44780_MODE_5X10 0x04
#define HD44780_MODE_5X8 0x00

#define HD44780_SET_CGRAM_ADDR_COMMAND_5X8(IDX, ROW) (0x40 | ((IDX & 7) << 3) | (ROW & 7))

#define HD44780_SET_CURSOR_COMMAND 0x80
#define HD44780_SET_CURSOR_COMMAND_HOME HD44780_SET_CURSOR_COMMAND
#define HD44780_SET_CURSOR_COMMAND_2_LINE(COL, ROW) (HD44780_SET_CURSOR_COMMAND | ((COL & 0x3F) + ((ROW & 1) ? 0x40 : 0)))
#define HD44780_SET_CURSOR_COMMAND_1_LINE(COL) (HD44780_SET_CURSOR_COMMAND | (COL & 0x7F))

//Busy flag when reading from controller
#define HD44780_BUSYFLAG 0x80

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the LCD in the given mode. Only supports 8-bit data
 * transfers for now.
 */
void hd44780_init(uint8_t mode);

/**
 * @brief Sends the given command to the controller.
 */
void hd44780_send_command(uint8_t command);

/**
 * @brief Writes a single byte to the cursor position, then increments
 * the cursor position.
 */
void hd44780_send_data(uint8_t data);
bool hd44780_is_busy();
void hd44780_block_until_ready();

#ifdef __cplusplus
}
#endif

#endif