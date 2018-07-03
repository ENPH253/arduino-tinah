#ifndef TINAH_PINS_H
#define TINAH_PINS_H

#include <stdint.h>

namespace TINAH {

namespace pins {
	constexpr uint8_t knob6 = 6;
	constexpr uint8_t knob7 = 7;

	constexpr uint8_t stopButton = 49;
	constexpr uint8_t startButton = 50;

	constexpr uint8_t servo[] = { 35, 31, 42 };

	constexpr uint8_t hBridgeDirection[] = { 24, 25, 38, 39 };
	constexpr uint8_t hBridgeEnable[] = { 29, 30, 36, 37 };
}

}

#endif