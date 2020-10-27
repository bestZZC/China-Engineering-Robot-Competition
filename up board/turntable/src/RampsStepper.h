#ifndef RAMPSSTEPPER_H_
#define RAMPSSTEPPER_H_

class RampsStepper {
public:
  RampsStepper(int aStepPin, int aDirPin, int aEnablePin);
  void enable(bool value = true);
  void disable();
  
  bool isOnPosition() const;
  int getPosition() const;
  void setPosition(int value);
  void stepToPosition(int value);
  void stepRelative(int value);
  void setCurrentInitialPosition();
  void xiaozhuanpan_xdu(float x);
  void xiaozhuanpan_dou();
  void dazhuanpan_terminal(float x);
  void dazhuanpan(float x);
  void jxb_zhuandongX(float x);


  float getPositionRad() const;
  void setPositionRad(float rad);
  void stepToPositionRad(float rad);
  void stepRelativeRad(float rad);
  
  void update();
  void updateSlow();
  void updateSlower();
  void updateSlowest();
  void updateFast();
  void updateFaster();

  void setReductionRatio(float gearRatio, int stepsPerRev);
private:
  int stepperStepTargetPosition;
  int stepperStepPosition;
    
  int stepPin;
  int dirPin;
  int enablePin;  
  
  float radToStepFactor;
};





#endif
