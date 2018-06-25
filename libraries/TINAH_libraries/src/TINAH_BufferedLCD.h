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

#ifndef TINAH_BUFFEREDLCD_H
#define TINAH_BUFFEREDLCD_H

#include <Print.h>

namespace TINAH {

/**
 * @brief Print-implementing HD44780 LCD controller class that utilizes a buffer
 * to improve performance.
 */
class BufferedLCD : public Print {
public:
	/**
	 * @brief Constructor for BufferedLCD. @ref begin still must be called
	 * to initialize this class and the display.
	 *
	 * @note It only makes sense to have one BufferedLCD, usually as a global
	 * object.
	 */
	BufferedLCD() = default;

	/**
	 * @brief Initializes the LCD.
	 *
	 * @param deferUpdates If true, calling write will return quickly, but
	 * characters may not be sent immediately if the controller is busy.
	 * Instead, the user must repeatedly call the updateIncremental method. If
	 * false, characters will be sent to the display immediately, but the write
	 * method will block waiting for the display controller to become free.
	 *
	 * @param useBusyFlag If true, the busy flag will be read from the
	 * controller to determine when it is not busy. If false, a conservative
	 * delay of 50 microseconds will be used since the previous byte sent to the
	 * controller instead. Enabling this option is typically twice as fast as
	 * not enabling.
	 *
	 * If useBusyFlag is enabled without deferUpdates, and the display is
	 * removed, the code will be stuck in a loop waiting for the controller to
	 * signal it is ready, but can be worked around by putting a pulldown
	 * resistor on the most significant bit of the data pins.
	 */
	void begin(bool deferUpdates = false, bool useBusyFlag = false);

	/**
	 * @brief Don't use this. For compatibility with LiquidCrystal, but the
	 * parameters do nothing as this library is hardcoded for size rows, cols
	 * below.
	 */
	inline void begin(int, int) {
		begin(false, false);
	}

	/**
	 * @brief Clears the display.
	 */
	void clear();

	/**
	 * @brief Equivalent to setCursor(0, 0).
	 */
	void home();

	/**
	 * @brief Sets the cursor so the next character printed is at the location
	 * col,row.
	 */
	void setCursor(uint8_t col, uint8_t row);

	/**
	 * @brief Writes a byte at the cursor position, then increments the cursor
	 * position.
	 *
	 * Handles carriage returns by moving the cursor to the beginning of the
	 * line.
	 *
	 * Handles newlines by clearing the contents of the line after the cursor,
	 * then moving the curor to the beginning of the other line.
	 *
	 * Handles line wrapping by moving the cursor to the beginning of the other
	 * line once a character that would be off the screen is about to be
	 * printed.
	 *
	 * @returns 1 always.
	 */
	virtual size_t write(uint8_t);

	/**
	 * @brief Performs deferred updates of the LCD controller, without blocking.
	 *
	 * When deferUpdates = true, this method must be called repeatedly to send
	 * data to the LCD controller. Call it one or more times in your program's
	 * main loop and the display will update character by character.
	 *
	 * When deferUpdates = false, this method does nothing.
	 *
	 * @retval true if this function should be called again to send data to the
	 * LCD controller.
	 * @retval false if the LCD controller is fully up-to-date with the buffer.
	 */
	bool updateIncremental();

	/**
	 * @brief Fully updates the contents of the LCD controller without blocking.
	 *
	 * When deferUpdates == false, this method does nothing.
	 */
	void flush();

	/**
	 * @brief Loads a custom 5x8 character on the controller in one of 8 slots.
	 *
	 * Once loaded, they can be displayed by calling write with the index they
	 * are loaded into. This method blocks until all the data is loaded.
	 *
	 * @param index The index to load into. Must be >= 0 and < 8.
	 * @param pixelData An array of 8 bytes where each set bit indicates
	 * an active pixel. The first byte is the top row of the character,
	 * and the 5 least significant bits are used, with the least significant
	 * on the right.
	 *
	 * Example usage:
	 * const uint8_t B[] = {
	 *     0b11111,
	 *     0b10011,
	 *     0b10101,
	 *     0b10011,
	 *     0b10101,
	 *     0b10011,
	 *     0b11111,
	 *     0b00000
	 * }
	 * LCD.loadCustomCharacter(0, B);
	 * LCD.print("Li\10uid Cry\10tal");
	 */
	void loadCustomCharacter(uint8_t index, const uint8_t pixelData[8]);

	/**
	 * @brief Version of loadCustomCharacter that works with pixel data in
	 * program memory.
	 *
	 * Once loaded, they can be displayed by calling write with the index they
	 * are loaded into. This method blocks until all the data is loaded.
	 *
	 * @param index The index to load into. Must be >= 0 and < 8.
	 * @param progmemPixelData Located in program memory, see loadCustomCharacter
	 * for format.
	 *
	 * Example usage:
	 * static const uint8_t B[] PROGRMEM = {
	 *     0b11111,
	 *     0b10011,
	 *     0b10101,
	 *     0b10011,
	 *     0b10101,
	 *     0b10011,
	 *     0b11111,
	 *     0b00000
	 * }
	 * LCD.loadCustomCharacterProgmem(0, B);
	 * LCD.print(F("Li\10uid Cry\10tal"));
	 */
	void loadCustomCharacterProgmem(uint8_t index, const uint8_t progmemPixelData[8]);

private:
	static constexpr uint8_t defaultDelay = 50;
	static constexpr uint8_t rowStride = 0x40;
	static constexpr uint8_t rows = 2;
	static constexpr uint8_t cols = 16;

	bool useBusyFlag;
	bool deferUpdates;

	bool deferredReturn;
	uint8_t cursorAddr;
	uint8_t updateAddr;
	uint8_t screenAddr;
	uint8_t buffer[rows * rowStride];
	uint8_t screenContents[rows * rowStride];
	unsigned long lastCommandTime;
	uint16_t commandDelay;

	void putChar(uint8_t character);
	void sendCommand(uint8_t command, uint8_t delay = defaultDelay);
	void sendData(uint8_t data);
	bool busy();
};

}

#endif
