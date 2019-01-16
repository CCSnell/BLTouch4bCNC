
/* This tests a BL-TOUCH auto leveling sensor for bCNC. 
 *  It sends a RC servo PWM signal of 10 degrees to lower the test probe.
 *  If the probe is pushed in approximately 0.130", the unit sends a quick positive pulse
 *  on the white wire. No more than 3.3 volts. (That is why pullups on)
 *  The unit will suck the probe in & flash the red light.
 *  Then this sends a PWM signal of 90 degrees to raise the test probe, simulating moving 
 *  to another test point. After a delay it lowers the probe again & starts over.
 *  BL-touch may have inverted output so use the attachInterrupt line for FALLING edge detect.
 *  The repeatability on this one ranged about 0.003"
 *  This example code is in the public domain. Original code by DNA-Robotics.com 2018
 *  https://forum.arduino.cc/index.php?topic=549369.0v
*/ 

#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 
int Touch = 0; 
int vals = 0; 
int valsMap = 0;
const byte interruptPin = 2;  // White probe output wire to Digital pin 2
const int linkPin = A1;       // the pin that COOLANT_ENABLE is attached to 
void setup() 
{ 
  myservo.attach(3);  // attaches the servo on pin 3 to the servo object 
  pinMode(ledPin, OUTPUT);
  pinMode(linkPin, INPUT_PULLUP);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);  // off to on will interrupt & go to blink function
} 
 
void loop() 
{ 
  probeWait:
  vals = map(analogRead(linkPin), 0, 1020, 0, 1);
  if (vals == 0)
  {
   delay(200); 
   myservo.write(10);              // tell servo to go to DOWN position 
   delay(15);                       // waits 15ms for the servo to reach the position
   goto probeRun;
  }

  else  
  {
    myservo.write(90);        // tell servo to go to UP position. 
    goto probeWait;
  }

  probeRun:
  if (Touch == 1)       // went from off to on:
    {
      myservo.write(90);        // tell servo to go to UP position. 
      delay(200);
      Touch = 0; 
    }
}

void blink() {
  Touch = 1;
}
