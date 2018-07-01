#include "TINAH_HBridge.h"
#include <Arduino.h>

using namespace TINAH;


constexpr uint8_t HBridge::directionPins[numChannels] = { 24, 25, 38, 39 };
constexpr uint8_t HBridge::enablePins[numChannels] = { 29, 30, 36, 37 };

void HBridge::init() {
	for (int i = 0; i < numChannels; i++) {
		pinMode(directionPins[i], OUTPUT);
		pinMode(enablePins[i], OUTPUT);
	}
	stopAll();
}

void HBridge::setOutput(uint8_t channel, int16_t output) {
	uint8_t dutyCycle = (abs(output) > 255) ? 255 : abs(output);
	digitalWrite(directionPins[channel], output < 0 ? LOW : HIGH);
	analogWrite(enablePins[channel], dutyCycle);
}

void HBridge::stop(uint8_t channel) {
	setOutput(channel, 0);
}

void HBridge::stopAll() {
	for (int i = 0; i < numChannels; i++) {
		stop(i);
	}
}