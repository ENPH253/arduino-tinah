#ifndef TINAH_SERVO_H
#define TINAH_SERVO_H

#include <avr/interrupt.h>
#include <Arduino.h>

#define MAX_SERVO_CHANNELS    (8U)   /* don't change! */
#define MAX_SERVO_ANGLE       (180U)

namespace TINAH {

class Servo
{
public:

	/**
	 * @brief constructs a servo object that is not yet attached to the pin
	 *
	 * @param pin If provided, the object will automatically attach to this pin
	 * on the first use.
	 */
	Servo(uint8_t pin = 0xFF);

	/**
	 * @brief Enables the servo signal output on the given pin.
	 *
	 * @param pin The Arduino pin number to use.
	 *
	 * @returns true if the pin is valid and attaching was successful.
	 */
	uint8_t attach(uint8_t pin);

	/**
	 * @brief Enables the servo signal output.
	 *
	 * Uses the most recently used pin, or the one specified in the constructor
	 * if attach(pin) has never been used.
	 *
	 * @returns true if the pin is valid and attaching was successful.
	 */
	uint8_t attach(void);

	/**
	 * @brief Disables the servo signal output, should make the servo go "limp".
	 */
	void detach(void);

	/**
	 * @brief Returns true if the servo signal is currently enabled.
	 */
	bool attached(void);

	/**
	 * @brief Sets the servo output to the given angle
	 *
	 * Attaches the servo object if it was not already attached.
	 *
	 * Using this doesn't always correspond to the proper value in degrees
	 * if you are using a servo with a more limited range, or one with
	 * different pulse timing specifications.
	 *
	 * @param angle in degrees
	 */
	void write(uint16_t angle);
private:
	uint8_t pin;
	uint8_t channel;
	static uint8_t timerRunning;
	static uint8_t channelsUsed;

};

}

#endif
