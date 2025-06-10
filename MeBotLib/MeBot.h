/*
  Author = Joshua Muunoja Emilsson
  Note = Produced on 10/06/2025
  Version = 1.0
*/

#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>

// Forward declaration of Robot for use in OperatingMode
class Robot;

class Motor {
  private:
    int enableForwardPin;
    int phaseReversePin;
    int speed;
    String motorControlType;
    
  public:
    Motor();
    Motor(int enableForwardPinField, int phaseReversePinField, String motorControlTypeField);
  
    void setup();
    void stop();
    void forward(int speed);
    void reverse(int speed);
    void coast();
    void brake();
};

class OperatingMode {
  private:
    int id;
    
  public:
    // Function pointers to mode functions taking a pointer to a Robot instance
    void (*repeatFuncPtr)(Robot*);
    void (*startFuncPtr)(Robot*);
  
    OperatingMode();
    OperatingMode(int IDinput);
  
    void setRepeat(void (*newFunc)(Robot*));
    void repeat(Robot* robot);
  
    void setStart(void (*newFunc)(Robot*));
    void start(Robot* robot);
  
    int getID();
};

class Robot {
  private:
    Motor rightMotor;
    Motor leftMotor;
    int nOfModes;
    OperatingMode* operatingModes;
    int previousMode;
    int currentMode;
    
  public:
    // Constructor takes two Motor objects for the right and left motors
    Robot(Motor rightMotorField, Motor leftMotorField);
    
    void setup();
    void addMode(OperatingMode inputMode);
    void setMode(int mode);
    void run();
    
    // MOTOR/MOVEMENT FUNCTIONS
    void driveForward(int leftMotorSpeed, int rightMotorSpeed);
    void driveReverse(int leftMotorSpeed, int rightMotorSpeed);
    void spinRight(int leftMotorSpeed, int rightMotorSpeed);
    void spinLeft(int leftMotorSpeed, int rightMotorSpeed);
    void stop();
    void brake();
};

#endif
