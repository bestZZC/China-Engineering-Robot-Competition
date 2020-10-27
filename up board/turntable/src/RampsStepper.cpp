#include "rampsStepper.h"
#include <arduino.h>




RampsStepper::RampsStepper(int aStepPin, int aDirPin, int aEnablePin) {
  setReductionRatio(1, 3200);
  stepPin = aStepPin;
  dirPin = aDirPin;
  enablePin = aEnablePin;
  stepperStepPosition = 0;
  stepperStepTargetPosition;
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  enable(true);
}

void RampsStepper::enable(bool value) {
  digitalWrite(enablePin, !value);
}

void RampsStepper::disable() {
  digitalWrite(enablePin, HIGH);
}

bool RampsStepper::isOnPosition() const {
  return stepperStepPosition == stepperStepTargetPosition;
}

int RampsStepper::getPosition() const {
  return stepperStepPosition;
}

void RampsStepper::setCurrentInitialPosition(){
  stepperStepPosition = 0;
}

void RampsStepper::setPosition(int value) {
  stepperStepPosition = value;
  stepperStepTargetPosition = value;
}

void RampsStepper::stepToPosition(int value) {
  stepperStepTargetPosition = value;
}

void RampsStepper::stepRelative(int value) {
  value += stepperStepPosition;
  stepToPosition(value);
}

float RampsStepper::getPositionRad() const {
  return stepperStepPosition / radToStepFactor;
}

void RampsStepper::setPositionRad(float rad) {
  setPosition(rad * radToStepFactor);
}

void RampsStepper::stepToPositionRad(float rad) {
  stepperStepTargetPosition = rad * radToStepFactor;
}

void RampsStepper::stepRelativeRad(float rad) {
  stepRelative(rad * radToStepFactor);
}

void RampsStepper::xiaozhuanpan_xdu(float x){
  this->stepToPositionRad(this->getPositionRad() + x);
  this->updateFaster();
}

void RampsStepper::xiaozhuanpan_dou(){
  this->stepToPositionRad(this->getPositionRad() - 6*PI/180);
  this->updateSlowest();
 
  this->stepToPositionRad(this->getPositionRad() + 12*PI/180);
  this->updateSlowest();

  this->stepToPositionRad(this->getPositionRad() - 6*PI/180);
  this->updateSlowest();
  
}

void RampsStepper::dazhuanpan(float x){
  this->stepToPositionRad(this->getPositionRad() + x);
  this->updateFast();
}

void RampsStepper::dazhuanpan_terminal(float x){
  this->stepToPositionRad(x);
  this->updateFast();
}

//正顺时针，负逆时针
void RampsStepper::jxb_zhuandongX(float x){
  this->stepToPositionRad(this->getPositionRad() + x);//2.3
  this->updateSlower();
}

void RampsStepper::update() {   
  while (stepperStepTargetPosition < stepperStepPosition) {  
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(200);
    stepperStepPosition--;
  }
  while (stepperStepTargetPosition > stepperStepPosition) {    
    digitalWrite(dirPin, LOW);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(200);
    stepperStepPosition++;
  }
}

void RampsStepper::updateSlow() {   
  while (stepperStepTargetPosition < stepperStepPosition) {  
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(275);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(275);
    stepperStepPosition--;
  }
  while (stepperStepTargetPosition > stepperStepPosition) {    
    digitalWrite(dirPin, LOW);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(275);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(275);
    stepperStepPosition++;
  }
}

void RampsStepper::updateSlower() {   
  while (stepperStepTargetPosition < stepperStepPosition) {  
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(400);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(400);
    stepperStepPosition--;
  }
  while (stepperStepTargetPosition > stepperStepPosition) {    
    digitalWrite(dirPin, LOW);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(400);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(400);
    stepperStepPosition++;
  }
}


void RampsStepper::updateSlowest() {   
  while (stepperStepTargetPosition < stepperStepPosition) {  
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(600);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);
    stepperStepPosition--;
  }
  while (stepperStepTargetPosition > stepperStepPosition) {    
    digitalWrite(dirPin, LOW);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(600);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);
    stepperStepPosition++;
  }
}



void RampsStepper::updateFast() {   
  while (stepperStepTargetPosition < stepperStepPosition) {  
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
    stepperStepPosition--;
  }
  while (stepperStepTargetPosition > stepperStepPosition) {    
    digitalWrite(dirPin, LOW);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
    stepperStepPosition++;
  }
}

void RampsStepper::updateFaster() {   
  while (stepperStepTargetPosition < stepperStepPosition) {  
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(50);
    stepperStepPosition--;
  }
  while (stepperStepTargetPosition > stepperStepPosition) {    
    digitalWrite(dirPin, LOW);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(50);
    stepperStepPosition++;
  }
}

void RampsStepper::setReductionRatio(float gearRatio, int stepsPerRev) {
  radToStepFactor = gearRatio * stepsPerRev / 2 / PI;
};
