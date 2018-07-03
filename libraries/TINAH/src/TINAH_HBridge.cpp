#include "TINAH_HBridge.h"
#include "TINAH_Pins.h"
#include <Arduino.h>

using namespace TINAH;

#define lengthof(ARR) (sizeof(ARR) / sizeof(decltype(ARR[0])))
static_assert(lengthof(pins::hBridgeDirection) == HBridge::numChannels, 
	"Wrong number of direction pins");
static_assert(lengthof(pins::hBridgeEnable) == HBridge::numChannels, 
	"Wrong number of enable pins");

HBridge::HBridge(uint8_t channel) :
	directionPin(pins::hBridgeDirection[channel]),
	enablePin(pins::hBridgeEnable[channel]) 
{
	init();
}

void HBridge::init() {
	pinMode(directionPin, OUTPUT);
	pinMode(enablePin, OUTPUT);
	off();
}

void HBridge::setOutput(int16_t outputLevel) {
	uint8_t dutyCycle = (abs(outputLevel) > 255) ? 255 : abs(outputLevel);
	digitalWrite(directionPin, outputLevel < 0 ? LOW : HIGH);
	analogWrite(enablePin, dutyCycle);
}

void HBridge::off() {
	setOutput(0);
}

void HBridge::allOff() {
	for (int i = 0; i < numChannels; i++) {
		digitalWrite(pins::hBridgeEnable[i], LOW);
	}
}