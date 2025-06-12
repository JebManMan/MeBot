/*
  Author = Joshua Muunoja Emilsson
  Note = Produced on 10/06/2025
  Version = 1.0
*/

#include "MeBot.h"

//========== Motor Class ==========
Motor::Motor() { }

Motor::Motor(int enableForwardPinField, int phaseReversePinField, String motorControlTypeField = "IN/IN") {
  speed = 0;
  enableForwardPin = enableForwardPinField;
  phaseReversePin = phaseReversePinField;
  motorControlType = motorControlTypeField;
}

void Motor::setup() {
  

  pinMode(enableForwardPin, OUTPUT);
  pinMode(phaseReversePin, OUTPUT);
}

void Motor::stop() {
  digitalWrite(phaseReversePin, LOW);
  digitalWrite(enableForwardPin, LOW);
}

void Motor::forward(int speed = 255) {
  if (motorControlType == "IN/IN") {
    digitalWrite(phaseReversePin, LOW);
    analogWrite(enableForwardPin, speed);
  } else if (motorControlType == "PHASE/ENABLE") {
    digitalWrite(phaseReversePin, LOW);
    analogWrite(enableForwardPin, speed);
  }
}

void Motor::reverse(int speed = 255) {
  if (motorControlType == "IN/IN") {
    digitalWrite(enableForwardPin, LOW);
    analogWrite(phaseReversePin, speed);
  } else if (motorControlType == "PHASE/ENABLE") {
    digitalWrite(phaseReversePin, HIGH);
    analogWrite(enableForwardPin, speed);
  }
}

void Motor::coast() {
  if (motorControlType == "IN/IN") {
    digitalWrite(enableForwardPin, LOW);
    digitalWrite(phaseReversePin, LOW);
  } else if (motorControlType == "PHASE/ENABLE") {
    digitalWrite(phaseReversePin, LOW);
    digitalWrite(enableForwardPin, LOW);
  }
}

//This is for specific motor drivers that support braking in a IN/IN mode
//For home made H-bridges this will damage the transistors
void Motor::brake() {
  if (motorControlType == "IN/IN_Special") {
    digitalWrite(enableForwardPin, HIGH);
    digitalWrite(phaseReversePin, HIGH);
  } else if (motorControlType == "PHASE/ENABLE") {
    digitalWrite(enableForwardPin, LOW);
    digitalWrite(phaseReversePin, HIGH);
  }
}

//========== OperatingMode Class ==========
OperatingMode::OperatingMode() { }

OperatingMode::OperatingMode(int IDinput) {
  id = IDinput;
}

void OperatingMode::setRepeat(void (*newFunc)(Robot*)) {
  repeatFuncPtr = newFunc;
}

void OperatingMode::repeat(Robot* robot) {
  Serial.println("REPEAT CALLED");
  if (repeatFuncPtr) {
    repeatFuncPtr(robot);
  }
}

void OperatingMode::setStart(void (*newFunc)(Robot*)) {
  startFuncPtr = newFunc;
}

void OperatingMode::start(Robot* robot) {
  if (startFuncPtr) {
    startFuncPtr(robot);
  }
}

int OperatingMode::getID() {
  return id;
}

//========== Robot Class ==========
Robot::Robot(Motor rightMotorField, Motor leftMotorField)
  : rightMotor(rightMotorField), leftMotor(leftMotorField), nOfModes(1),
    operatingModes(nullptr), previousMode(0), currentMode(0)
{ }

void Robot::setup() {
  rightMotor.setup();
  leftMotor.setup();
}

void Robot::addMode(OperatingMode inputMode) {
  OperatingMode* newArray = new OperatingMode[nOfModes + 1];
  
  if (operatingModes != nullptr) {
    for (int i = 0; i < nOfModes; i++) {
      newArray[i] = operatingModes[i];
    }
    delete[] operatingModes;  // Free old memory if allocated
  }
  
  newArray[nOfModes] = inputMode;
  nOfModes++;
  operatingModes = newArray;
}

void Robot::setMode(int mode) {
  currentMode = mode;
}

void Robot::run() {
  for (int i = 0; i < nOfModes; i++) {
    if (currentMode == operatingModes[i].getID()) {
      if (currentMode != previousMode) {
        operatingModes[i].start(this);
        previousMode = currentMode;
      }
      operatingModes[i].repeat(this);
    }
  }
}

//========== Robot Movement Functions ==========
void Robot::driveForward(int leftMotorSpeed=255, int rightMotorSpeed=255) {
  rightMotor.forward(rightMotorSpeed);
  leftMotor.forward(leftMotorSpeed);
}

void Robot::driveReverse(int leftMotorSpeed=255, int rightMotorSpeed =255) {
  leftMotor.reverse(leftMotorSpeed);
  rightMotor.reverse(rightMotorSpeed);
}

void Robot::spinRight(int leftMotorSpeed=255, int rightMotorSpeed=255) {
  leftMotor.forward(leftMotorSpeed);
  rightMotor.reverse(rightMotorSpeed);
}

void Robot::spinLeft(int leftMotorSpeed=255, int rightMotorSpeed=255) {
  leftMotor.reverse(leftMotorSpeed);
  rightMotor.forward(rightMotorSpeed);
}

void Robot::stop() {
  leftMotor.stop();
  rightMotor.stop();
}

void Robot::brake() {
  leftMotor.brake();
  rightMotor.brake();
}


