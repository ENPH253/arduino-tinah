#include <phys253.h>
#include <LiquidCrystal.h>

////////////////////////////////////////////////
// TINAH Demo Program - UBC Engineering Physics 253
// (nakane, 2016 May 5) - minor updates for new ServoTINAH setup.   
// (nakane, 2015 May 7) -  Update for IDE 1.6.4
// (nakane, 2015 Jan 2)  - Updated for use in Arduino IDE (1.0.6)
/////////////////////////////////////////////////

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;
}




void loop()
{

  int variable = LOW ;                // setting up a few temporary variables
  int value, value2, i, pinNumber ;
  int portCounter = 0 ;
  int motorSpeed = 0 ;


  LCD.clear();  LCD.home() ;
  // the next comment line is used to help identify when lines written to the LCD will be cut off.
  // 16 positions on LCD screen:   1234567890123456
  LCD.setCursor(0, 0); LCD.print("TINAH Demo Pgm");
  LCD.setCursor(0, 1); LCD.print("Arduino IDE '16");
  delay(1000)  ;



  /////////////////////////////////////////////////
  // DIGITAL INPUT TEST
  /////////////////////////////////////////////////
  LCD.clear();  LCD.home() ;
  // 16 positions on LCD screen:   1234567890123456
  LCD.setCursor(0, 0); LCD.print("Digital Inputs, ");
  LCD.setCursor(0, 1); LCD.print("press, <stop>");
  while ( !(stopbutton()) ) ;              // a convenient way to wait at a screen until the user presses the start button



  /* This loop outputs the binary value at each of the two digital ports.
  Port 0 is read and written as the entire port, while Port 1 in this loop is read pin by pin, from 15 down to 8.
  Port 1 could have been read and written in exactly the same way as Port 0.
  Note the difference when the output changes in pin 15 versus pin 8:
  -when pin 15 = 0, the output value of 0 is sent to the screen
  -when pin 8 = 0, the output value is truncated from the value written to the screen (since the entire port value at port 0 is being treated like one big number */

  while ( ! (startbutton()  ) )
  {
    LCD.clear();  LCD.home() ;
    // 16 positions on LCD screen:     1234567890123456
    variable = portRead(0) ;
    LCD.setCursor(0, 0); LCD.print("P0:" );  LCD.print(variable, BIN) ;
    LCD.setCursor(0, 1); LCD.print("P1:" );
    for (pinNumber = 15; pinNumber >= 8; pinNumber-- )
    {
      variable = digitalRead(pinNumber) ;
      if (variable == HIGH)
      {
        LCD.print("1") ;
      }
      else
      {
        LCD.print("0") ;
      }
    }
    LCD.print("<st>");
    delay(200) ;
  }


  /////////////////////////////////////////////////
  // ANALOG TEST INPUT
  /////////////////////////////////////////////////
  LCD.clear();  LCD.home() ;
  // 16 positions on LCD screen:   1234567890123456
  LCD.setCursor(0, 0); LCD.print("Analog Inputs");
  LCD.setCursor(0, 1); LCD.print("/16  <stop>");
  while ( !(stopbutton()) ) ;

  /* This loop outputs the values from the 8 analog inputs onto a single screen.
  Note that the values are divided by 16.  This was done so that each 10-bit input (normally 2^10 = 1024) would only reach a maximum of 1024/16 = 64,
  so that all 8 values could be written to the screen at the same time.
  Also note that Analog inputs 7 and 6 are usually tied to the two knobs on the board.  The jumpers, both next to the knobs, can be moved in order to
  maximize the # of analog inputs to 8, but normally leave the knobs as the analog inputs, if possible.
  */


  while ( ! (startbutton()  ) )
  {
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print(analogRead(7) / 16) ;  LCD.print(" ") ;
    LCD.print(analogRead(6) / 16) ;  LCD.print(" ") ;
    LCD.print(analogRead(5) / 16) ;  LCD.print(" ") ;
    LCD.print(analogRead(4) / 16) ;  LCD.print(" ") ;
    LCD.setCursor(0, 1);
    LCD.print(analogRead(3) / 16) ;  LCD.print(" ") ;
    LCD.print(analogRead(2) / 16) ;  LCD.print(" ") ;
    LCD.print(analogRead(1) / 16) ;  LCD.print(" ") ;
    LCD.print(analogRead(0) / 16) ;  LCD.print("<st>");
    delay(200) ;
  }


  /////////////////////////////////////////////////
  // DC MOTOR TEST
  /////////////////////////////////////////////////

  /*  This program uses the two knob inputs and cycles through the 4 motor outputs, positive and negative.
  You can see by the timing and brightness of the LED the direction and speed of the motor.
  Knob 6 sets the PWM duty cycle of the DC motor signal
  knob 7 sets the length of time that the motor spends going in each direction for each motor.
  */

  while ( !(stopbutton()) ) {
    for (int i = 0; i < 4; i++) {
      motorSpeed = knob(6) / 4 ;
      LCD.clear();  LCD.home() ;
      // 16 positions on LCD screen:   1234567890123456
      LCD.setCursor(0, 0); LCD.print("DC Motor: ");  LCD.print(motorSpeed) ;
      LCD.setCursor(0, 1); LCD.print("<stop>");

      motor.speed(i, motorSpeed) ;
      if (stopbutton()) {
        break;
      } ;
      delay(knob(7)) ;
      motor.speed(i, -motorSpeed);
      if (stopbutton()) {
        break;
      } ;
      delay(knob(7)) ;
      motor.stop(i) ;
    }
  }
  motor.stop_all() ;


  /////////////////////////////////////////////////
  // SERVO MOTOR TEST
  /////////////////////////////////////////////////

  /*
  Check online to see the correct configuration of the motor to the board.
  DO NOT ACCIDENTALLY PLUG THE SERVO MOTOR BACKWARDS INTO THE CONNECTOR!
  All 3 servo motors are updated with the same position.
  A maximum of 180 is sent to the servo motors; the command RCServo#.write(val) sends the servo to approximately an angular position of val.
  Knob 6 sets the position of the servo motor
  knob 7 sets the delay betweeen updates to the servo motor output
  */
  
  while ( !(startbutton()) ) {
    variable = map(knob(6), 0, 1023, 0, 180) ;
    RCServo0.write(variable);
    RCServo1.write(variable);
    RCServo2.write(variable);
    LCD.clear();  LCD.home() ;
    // 16 positions on LCD screen:   1234567890123456
    LCD.setCursor(0, 0); LCD.print("Servos " ); LCD.print(variable) ;
    LCD.setCursor(0, 1); LCD.print("knobs + <start>");
    delay(knob(7)) ;
  }



  LCD.clear();  LCD.home() ;
  // 16 positions on LCD screen:   1234567890123456
  LCD.setCursor(0, 0); LCD.print("Testing Complete");
  LCD.setCursor(0, 1); LCD.print("<stop> to repeat");
  while ( !(stopbutton()) ) ;

}

