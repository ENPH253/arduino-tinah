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

#ifndef TINAH_HBRIDGE_H
#define TINAH_HBRIDGE_H
#include <stdint.h>

namespace TINAH {

class HBridge {
public:

	/**
	 * @brief The total number of H-bridge outputs available on the TINAH.
	 */
	static constexpr uint8_t numChannels = 4;

	/**
	 * @brief Constructs an H-bridge object for the given channel.
	 * 
	 * Sets the control pins for the H-bridge to output and turns off the
	 * H-bridge.
	 * 
	 * @param channel Should be less than @ref numChannels.
	 */
	HBridge(uint8_t channel);

	/**
	 * Sets the control pins for the H-bridge to output and turns off the
	 * H-bridge.
	 */
	void init();

	/**
	 * @brief Sets the output duty cycle and direction for an H-bridge.
	 *
	 * @param outputLevel The duty cycle to output. 0 is off, 255 is full duty 
	 * cycle in one direction, and -255 is full duty cycle in the other 
	 * direction.
	 */
	void setOutput(int16_t outputLevel);

	/**
	 * @brief Turs off the output on the given channel.
	 */
	void off();

	/**
	 * @brief Turns off all channels.
	 */
	static void allOff();

private:
	const uint8_t directionPin;
	const uint8_t enablePin;
};

}

#endif