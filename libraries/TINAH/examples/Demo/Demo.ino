#include <TINAH_Pins.h>
#include <TINAH_BufferedLCD.h>
#include <TINAH_HBridge.h>
#include <TINAH_Servo.h>

// Alternatively, this includes all of the above files.
// #include <TINAH.h>

/*
 * Declare object for liquid crystal display.
 * 
 * You must later call begin for it to work.
 */
TINAH::BufferedLCD LCD;

//Declare objects for DC motors. Use motor output channel as parameter.
TINAH::HBridge leftMotor(2);
TINAH::HBridge rightMotor(3);

/**
 * Declare objects for servo motors. Use the predefined pins from the 
 * TINAH::pins::servo array, or choose your own - the Servo library doesn't need
 * the pins to have PWM.
 */
TINAH::Servo servo0(TINAH::pins::servo[0]);
TINAH::Servo servo1(15);

void setup() {
  /*
   * Initializes the BufferedLCD class and sends setup commands to the display.
   * 
   * Note that the size of the display is no longer provided, it is fixed to 16x2.
   * 
   * The first argument is true to use the deferred update mode. Deferred update
   * mode requires calling the updateIncremental method while not using it means
   * text updates immediately, but printing will block other code as data is sent.
   * 
   * The second argument is false to use time delays rather than reading the busy
   * status from the LCD. You can change this to be true to make LCD writes faster,
   * but if the LCD is not present or malfunctions, the code will get stuck waiting 
   * for it to become ready in non-deferred mode.
   */
  LCD.begin(true, false);

  LCD.setCursor(0, 0);
  //Wrap strings for printing in F() to save RAM.
  //Newlines should work properly now
  LCD.print(F("TINAH Demo\nStart for motor"));

  /*
   * When using the deferred update mode, flush() makes sure the contents are fully
   * displayed after the call returns.
   */
  LCD.flush();
  delay(500);
  LCD.clear();
}

bool motorEnabled = false;

void loop() {
  //If you don't know the correct type to use for a variable, auto can guess.
  auto loopStart = millis();
  
  int16_t knobA = analogRead(6);
  int16_t knobB = analogRead(7);
  uint8_t servoOutput = ((int32_t)knobA * 180) / 1024;
  int16_t motorOutput = (knobB - 512) / 2;
  
  if (!digitalRead(TINAH::pins::startButton)) {
    motorEnabled = true;
  }  
  if (!digitalRead(TINAH::pins::stopButton)) {
    motorEnabled = false;
  }

  servo0.write(servoOutput);
  if (motorEnabled) {
    leftMotor.setOutput(motorOutput);
    rightMotor.setOutput(-motorOutput);
  }
  else {
    TINAH::HBridge::allOff();
  }

  LCD.clear();

  LCD.setCursor(0, 0);
  LCD.print(F("A:"));
  LCD.print(knobA);
  LCD.setCursor(0, 1);
  LCD.print(F("B:"));
  LCD.print(knobB);

  LCD.setCursor(6, 0);
  LCD.print(F("Servo:"));
  LCD.print(servoOutput);

  LCD.setCursor(6, 1);
  LCD.print(F("Motor:"));
  if (motorEnabled) {
    LCD.print(motorOutput);
  }
  else {
    LCD.print(F("off"));
  }

  //Loop speed regulation, and at the same time updating the LCD
  while (millis() < loopStart + 10) {
    //If there is nothing left to do this method returns false.
    LCD.updateIncremental();
  }

}
