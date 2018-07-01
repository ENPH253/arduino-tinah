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

#include "TINAH_BufferedLCD.h"
#include <Arduino.h>
#include <hd44780.h>

using namespace TINAH;

void BufferedLCD::begin(bool deferUpdates, bool useBusyFlag) {
	this->deferUpdates = deferUpdates;
	this->useBusyFlag = useBusyFlag;
	hd44780_init(HD44780_MODE_2_LINE | HD44780_MODE_5X8);
	sendCommand(HD44780_DISP_CTRL_COMMAND | HD44780_DISP_CTRL_DISPLAY_ON);
	sendCommand(HD44780_SET_CURSOR_COMMAND);
	cursorAddr = 0;
	screenAddr = 0;
	updateAddr = 0;
	for (int i = 0; i < rowStride * rows; i++) {
		buffer[i] = ' ';
		screenContents[i] = ' ';
	}
}

void BufferedLCD::clear() {
	if (deferUpdates) {
		for (uint8_t j = 0; j < rows; j++) {
			for (uint8_t i = 0; i < cols; i++) {
				buffer[i + j * rowStride] = ' ';
			}
		}
	}
	else {
		for (uint8_t j = 0; j < rows; j++) {
			setCursor(0, j);
			for (uint8_t i = 0; i < cols; i++) {
				putChar(' ');
			}
		}
	}
}

void BufferedLCD::home() {
	setCursor(0, 0);
}

void BufferedLCD::setCursor(uint8_t col, uint8_t row) {
	cursorAddr = (col % cols) + (row % rows) * rowStride;
}

size_t BufferedLCD::write(uint8_t data) {
	if (data == '\r') {
		deferredReturn = true;
	}
	else if (data == '\n') {
		deferredReturn = false;
		uint8_t lineEnd = (cursorAddr - cursorAddr % rowStride) + cols;
		while (cursorAddr < lineEnd) {
			putChar(' ');
		}
		cursorAddr = (cursorAddr + rowStride - cursorAddr % rowStride) % (rows * rowStride);
	}
	else {
		if (deferredReturn) {
			cursorAddr -= cursorAddr % rowStride;
			deferredReturn = false;
		}
		if (cursorAddr % rowStride >= cols) {
			cursorAddr = (cursorAddr + rowStride - cursorAddr % rowStride) % (rows * rowStride);
		}
		putChar(data);
	}
	return 1;
}

void BufferedLCD::sendCommand(uint8_t command, uint8_t delay) {
	while (busy());
	if (!useBusyFlag) {
		lastCommandTime = micros();
	}
	hd44780_send_command(command);
	if (!useBusyFlag) {
		commandDelay = delay;
	}
}

void BufferedLCD::sendData(uint8_t data) {
	while (busy());
	if (!useBusyFlag) {
		lastCommandTime = micros();
	}
	hd44780_send_data(data);
	if (!useBusyFlag) {
		commandDelay = defaultDelay;
	}
}

void BufferedLCD::putChar(uint8_t character) {
	buffer[cursorAddr] = character;
	if (!deferUpdates) {
		if (buffer[cursorAddr] != screenContents[cursorAddr]) {
			if (screenAddr != cursorAddr) {
				sendCommand(HD44780_SET_CURSOR_COMMAND | (cursorAddr % (rowStride * rows)));
				screenAddr = cursorAddr;
			}
			sendData(character);
			screenContents[cursorAddr] = character;
			screenAddr++;
		}
	}
	cursorAddr++;
}

bool BufferedLCD::busy() {
	if (useBusyFlag) {
		return hd44780_is_busy();
	}
	else {
		return micros() - lastCommandTime < commandDelay;
	}
}

bool BufferedLCD::updateIncremental() {
	if (!deferUpdates) return false;
	for (int i = 0; i < rows * cols; i++) {
		if (buffer[updateAddr] != screenContents[updateAddr]) {
			if (busy()) return true;
			if (screenAddr == updateAddr) {
				sendData(buffer[updateAddr]);
				screenContents[updateAddr] = buffer[updateAddr];
				screenAddr++;
				updateAddr++;
				if (updateAddr % rowStride >= cols) {
					updateAddr = (updateAddr + rowStride - updateAddr % rowStride) % (rows * rowStride);
				}
				return true;
			}
			else {
				sendCommand(HD44780_SET_CURSOR_COMMAND | updateAddr);
				screenAddr = updateAddr;
				return true;
			}
		}
		updateAddr++;
		if (updateAddr % rowStride >= cols) {
			updateAddr = (updateAddr + rowStride - updateAddr % rowStride) % (rows * rowStride);
		}
	}
	updateAddr = 0;
	return false;
}

void BufferedLCD::flush() {
	while (updateIncremental());
}

void BufferedLCD::loadCustomCharacter(uint8_t index, const uint8_t pixelData[8]) {
	while (busy());
	sendCommand(HD44780_SET_CGRAM_ADDR_COMMAND_5X8(index, 0));
	for (int i = 0; i < 8; i++) {
		while (busy());
		sendData(pixelData[i]);
	}
	screenAddr = 0xFF;
}

void BufferedLCD::loadCustomCharacterProgmem(uint8_t index, const uint8_t progmemPixelData[8]) {
	uint8_t ramCopy[8];
	for (int i = 0; i < 8; i++) {
		ramCopy[i] = pgm_read_byte(&(progmemPixelData[i]));
	}
	loadCustomCharacter(index, ramCopy);
}