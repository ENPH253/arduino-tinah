#include <Arduino.h>
#include "phys253.h"
#include "motor.h"


LiquidCrystal LCD(26,27,28,16, 17, 18, 19,20,21,22,23);


motorClass motor ;

ServoTINAH RCServo0;    // declare variables for up to eight servos.   Replaced old Servo253 implementation 2015Jan2
ServoTINAH RCServo1;
ServoTINAH RCServo2;

int knob(int value) 		{ return analogRead(knobInput[value]) ;}
void buzzer	(int value) { return ;} //   pulseOut(buzzerOutput, value*2) ;}
void buzzerOff () 	    { return ;} //   pulseStop(buzzerOutput ) ;}
int startbutton() 		{ return !digitalRead(startInput) ;}
int stopbutton() 		{ return !digitalRead(stopInput) ;}

void portMode(int portval, int value)  {
  for (int tempval = 0; tempval< 8; tempval ++)  {
    pinMode(portval*8 + tempval, value) ;
  }
}

void portWrite(int portval, int value) {
  for (int tempval = 0; tempval< 8; tempval ++)  {
     digitalWrite(portval*8 + tempval, value & tempval) ;
  }
}

int  portRead(int portval) {
  int tempTotal = 0 ;
  int tempRead = 0 ;
  int interimVal = 1 ;
  int tempState ;
  for (int tempval = 0; tempval< 8; tempval ++)  {
     tempRead = digitalRead(portval*8 + tempval) ;
     if (tempRead == HIGH)
     {tempState = 1;}
     else
     {tempState = 0;}
   //  Serial.print(tempState) ;

     tempTotal = tempTotal + tempState * interimVal ;
//     Serial.print(' ') ;
//     Serial.print (interimVal) ;
     interimVal =  interimVal * 2 ;
//     Serial.print(' ') ;
//     Serial.print(tempTotal) ;
     }
  Serial.println(' ') ;
  return tempTotal ;
}

