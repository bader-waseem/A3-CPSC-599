 /*
 * Bas on Tech - 28BYJ-48 stepper motor with ULN2003A driver
 * This course is part of the courses on https://arduino-tutorials.net
 *  
 * (c) Copyright 2018-2019 - Bas van Dijk / Bas on Tech
 * This code and course is copyrighted. It is not allowed to use these courses commerically
 * without explicit written approval
 * 
 * YouTube:    https://www.youtube.com/c/BasOnTech
 * Facebook:   https://www.facebook.com/BasOnTechChannel
 * Instagram:  https://www.instagram.com/BasOnTech
 * Twitter:    https://twitter.com/BasOnTech
 * 
 * ---------------------------------------------------------------------------
 *
 * Original code:
 * http://www.airspayce.com/mikem/arduino/AccelStepper/index.html by Mike McCauley
 *
 */

#include <AccelStepper.h>           // Load the AccelStepper library

#define motorPin1  10                // IN1 pin on the ULN2003A driver
#define motorPin2  11                // IN2 pin on the ULN2003A driver
#define motorPin3  12               // IN3 pin on the ULN2003A driver
#define motorPin4  13               // IN4 pin on the ULN2003A driver

int stepsPerRevolution = 64;        // steps per revolution
float degreePerRevolution = 11.25;  // degree per revolution

/*
 * AccelStepper::FULL2WIRE (2) means: 2 wire stepper (2 pins needed). 
 * AccelStepper::FULL3WIRE (3) means: 3 wire stepper, like a harddisk motor (3 pins needed). 
 * AccelStepper::FULL4WIRE (4) means: 4 wire stepper (4 pins needed). 
 * AccelStepper::HALF3WIRE (6) means: 3 wire half stepper, like a harddisk motor (3 pins needed) 
 * AccelStepper::HALF4WIRE (8) means: 4 wire half stepper (4 pins needed) 
 *
 * AccelStepper uses AccelStepper::FULL4WIRE (4 pins needed) by default.
 */
AccelStepper stepper(AccelStepper::FULL4WIRE, motorPin1, motorPin3, motorPin2, motorPin4);

#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3' ,'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 
char numBuff[2] = {' ', '\0'};
int money;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  Serial.begin(9600);               // initialise the serial monitor

  stepper.setMaxSpeed(200.0);      // set the max motor speed
  stepper.setAcceleration(1000.0);   // set the acceleration
  stepper.setSpeed(200);            // set the current speed
}

void loop() {
  char keyPressed = customKeypad.getKey();
  if (keyPressed != NO_KEY && !stepper.isRunning() && stepper.distanceToGo() == 0) {
    stepper.setCurrentPosition(0);
    stepper.setSpeed(200); // This is here because the above method call resets speed to 0
    numBuff[0] = keyPressed;
    money = atoi(numBuff);
    stepper.moveTo(fullRotationsToSteps(float(money)));
  }
  stepper.run();                    // start moving the motor
}

/*
 * Converts degrees to steps
 * 
 * 28BYJ-48 motor has 5.625 degrees per step
 * 360 degrees / 5.625 = 64 steps per revolution
 * 
 * Example with degToSteps(45):
 * (64 / 5.625) * 45 = 512 steps
 */
float degToSteps(float deg) {
  return (stepsPerRevolution / degreePerRevolution) * deg;
}

float fullRotationsToSteps(float rotations) {
  return degToSteps(360.0 * rotations);
}
