/* 
 Built on an Arduino Uno
 
 This sketch uses a 180° servo + Adafruit's HMC5883L triple-axis magnometer. 
 The sketch has the servo always point north.
 This was the start of the prototyping for my Minecraft-esque compass project. 
 
 This uses code from the example code in Adafruit's library:
    https://github.com/adafruit/Adafruit_HMC5883_Unified
 It also uses the Adafruit Sensor Library:
    https://github.com/adafruit/Adafruit_Sensor
 
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Servo.h> 

Servo servo;

// this is the LED built into the Arduino UNO to indicate if the compass heading is greater than the 180° the servo can point
int outOfRangeLED = 13;

// setup the mag sensor
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


void setup(void) 
{
  Serial.begin(9600);
  Serial.println("HMC5883 + Servo"); 
  Serial.println("");
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }

  /* Attach the servo to PWM pin 9 */
  servo.attach(9);
  
  // set up the LED as an output
  pinMode(outOfRangeLED, OUTPUT);     
  	
}

void loop(void) 
{
  /* Get a new sensor event */
  sensors_event_t event; 
  mag.getEvent(&event);
  	
  // The unit must be parallel to the ground, so that the Z axis is facing directly up, then calculate the direction in radians
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  float declinationAngle = 0.0698131701;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0) heading += 2*PI;
  
  // Check for wrap due to addition of declination.
  if(heading > 2*PI) heading -= 2*PI;
  
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  /* Print the heading via serial */
  Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
    
  /* if the heading is within the servo's range, set the servo and turn off the LED
   else, turn on the LED */
  if(headingDegrees <= 180)
  {
    servo.write(headingDegrees);
    digitalWrite(outOfRangeLED, LOW);
  } 
  else 
  {
    digitalWrite(outOfRangeLED, HIGH);
  }
  
  delay(300);
}


