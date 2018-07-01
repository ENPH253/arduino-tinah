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
	Servo();
	uint8_t attach(uint8_t);
	void detach(void);
	bool attached(void);
	void write(uint16_t);
	uint16_t read(void);
private:
	uint8_t pin;
	uint8_t channel;
	static uint8_t timerRunning;
	static uint8_t channelsUsed;

};

}

#endif
