#include <Arduino.h>
#include "RampsStepper.h"
#include "pinout.h"


//大转盘从进洞到出洞3*PI/5
//小转盘从颜色识别到洞口PI/10

RampsStepper Motor_bottom(X_STEP_PIN,X_DIR_PIN,X_ENABLE_PIN);//逆时针正
RampsStepper Motor_middle(Y_STEP_PIN,Y_DIR_PIN,Y_ENABLE_PIN);//逆时针正
RampsStepper Motor_top(Z_STEP_PIN,Z_DIR_PIN,Z_ENABLE_PIN);//向下为正
RampsStepper stepperHighZP(E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN);
RampsStepper stepperLowZP(E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN);
int i=0;
int colorData = -1;
int colorSrc[3];
int colorBase[5];

float password_in(int a){
  switch (a)
  {
  case 0:
    return green_in;
  case 1:
    return white_in;
  case 2:
    return red_in;
  case 3:
    return black_in;
  case 4:
    return blue_in;
  }
  return -1;
}

void line(){
  float a = (2.4-PI/2)/100;
  float b = 0.417/100;
  for (int i = 0; i < 101; i++)
  {
    Motor_bottom.stepToPositionRad(Motor_bottom.getPositionRad()-a);
    Motor_middle.stepToPositionRad(Motor_middle.getPositionRad()+b);
    Motor_bottom.updateFast();
    Motor_middle.update();
  }
}

void line_ace(){
  float a = (2.33-PI/2)/100;
  float b = 0.417/100;
  for (int i = 0; i < 101; i++)
  {
    Motor_bottom.stepToPositionRad(Motor_bottom.getPositionRad()-a);
    Motor_middle.stepToPositionRad(Motor_middle.getPositionRad()+b);
    Motor_bottom.updateFast();
    Motor_middle.update();
  }
}


void 
lashi(int color){
  while (1)
  {
    if(digitalRead(input) == HIGH){
      switch (color)
      {
      case 0:
        stepperLowZP.dazhuanpan_terminal(green_out);
        break;
      case 1:
        stepperLowZP.dazhuanpan_terminal(white_out);
        break;
      case 2:
        stepperLowZP.dazhuanpan_terminal(red_out);
        break;
      case 3:
        stepperLowZP.dazhuanpan_terminal(black_out);
        break;
      case 4:
        stepperLowZP.dazhuanpan_terminal(blue_out);
        break;
      }
      
      digitalWrite(output,HIGH);
      delay(300);
      digitalWrite(output,LOW);
      break;
    }
  }
}




void zhuazi_setup(){
  digitalWrite(IN1,HIGH);  
  digitalWrite(IN2,LOW);
  analogWrite(ENA,75); 
  delay(400);
  digitalWrite(IN1,LOW);  
  digitalWrite(IN2,HIGH);
  analogWrite(ENA,15);
  delay(200);
}

void Z_toZhuanPan(){
  Motor_top.stepToPositionRad(-9);
  Motor_top.updateFast();  
}

void Z_toTop(){
  Motor_top.stepToPositionRad(0);
  Motor_top.updateFaster();
}

void Z_down_zhuazi_jiazhu(){

  //夹紧爪子
  digitalWrite(IN2,HIGH);  
  digitalWrite(IN1,LOW);
  analogWrite(ENA,75);
  delay(600);
  Motor_top.stepToPositionRad(-42);
  Motor_top.updateFaster();
  digitalWrite(IN2,LOW);  
  digitalWrite(IN1,HIGH);
  analogWrite(ENA,15);
  delay(1000);
}

void zhuazi_songkai(){
  digitalWrite(IN2,HIGH);  
  digitalWrite(IN2,LOW);
  analogWrite(ENA,75);
  delay(1000);
}

void initialize(){
  while(digitalRead(weidong_3) == HIGH)
    {
      digitalWrite(Z_DIR_PIN, LOW);
      delayMicroseconds(5);
      digitalWrite(Z_STEP_PIN, HIGH);
      delayMicroseconds(30);
      digitalWrite(Z_STEP_PIN, LOW);
      delayMicroseconds(30);
    }
  while (digitalRead(weidong_1) == HIGH || digitalRead(weidong_2) == HIGH)
  {
    if (digitalRead(weidong_1) == HIGH)
    {
      digitalWrite(X_DIR_PIN, LOW);
      delayMicroseconds(5);
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(200);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(200);
    }
    if (digitalRead(weidong_2) == HIGH){
      digitalWrite(Y_DIR_PIN, HIGH);
      delayMicroseconds(5);
      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(100);
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(100);
    }
  }
  //设定XYZ当前位置均为零点
  Motor_bottom.setCurrentInitialPosition();
  Motor_middle.setCurrentInitialPosition();
  Motor_top.setCurrentInitialPosition();
}

void yanseshibie_check(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  colorSrc[0] = pulseIn(OUT, LOW);
  Serial.print("R= ");
  Serial.print(colorSrc[0]);
  delay(100);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  colorSrc[1] = pulseIn(OUT, LOW);
  Serial.print("  G= ");
  Serial.print(colorSrc[1]);
  delay(100);
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  colorSrc[2] = pulseIn(OUT, LOW);
  Serial.print("  B= ");
  Serial.print(colorSrc[2]);
  delay(100);
 
    if(colorSrc[1] < 40 && colorSrc[2] < 40){
      Serial.println("  白色");
      stepperLowZP.dazhuanpan_terminal(white_in);
      colorData = 1;
    }else if (colorSrc[0] > 80 && colorSrc[1] > 105 && colorSrc[2] > 105)
  {
    Serial.println("  黑色");
    stepperLowZP.dazhuanpan_terminal(black_in);
    colorData = 3;
  }else if (colorSrc[0] < colorSrc[2] && colorSrc[0] < colorSrc[1]){
      Serial.println("  红色");
      stepperLowZP.dazhuanpan_terminal(red_in);
      colorData = 2;
  }else if (colorSrc[2] < colorSrc[1])
      {
        Serial.println("  蓝色");
        stepperLowZP.dazhuanpan_terminal(blue_in);
        colorData = 4;
      }else if (colorSrc[2] > colorSrc[1])
      {
        Serial.println("  绿色");
        stepperLowZP.dazhuanpan_terminal(green_in);
        colorData = 0;
      }
      
  
  /*if (colorSrc[0] < 10 && colorSrc[1] < 10 && colorSrc[2] < 10) {
    Serial.println("  白色");
    stepperLowZP.dazhuanpan_terminal(white_in);
    colorData = 1;
  }
  else if(colorSrc[0] > 35 && colorSrc[1] > 50 && colorSrc[2] > 40) {
    Serial.println("  黑色");
    stepperLowZP.dazhuanpan_terminal(black_in);
    colorData = 3;
  }
  else {
    if (colorSrc[0] <10) {
      Serial.println("  红色");
      stepperLowZP.dazhuanpan_terminal(red_in);
      colorData = 2;
    }
    else if (colorSrc[0] > colorSrc[1] && colorSrc[2] - colorSrc[1] > 20){
      Serial.println("  绿色");
      stepperLowZP.dazhuanpan_terminal(green_in);
      colorData = 0;
    }
    else if (colorSrc[0] - colorSrc[2] > 30 && colorSrc[1] - colorSrc[2] > 40){
      Serial.println("  蓝色");
      stepperLowZP.dazhuanpan_terminal(blue_in);
      colorData = 4;
    }
  }*/
  delay(500);
}

void grab(){
  //Z轴伸下去,爪子夹住
  Z_down_zhuazi_jiazhu();
  //先调整一下角度
  Motor_middle.jxb_zhuandongX(7*PI/180);
  Motor_bottom.jxb_zhuandongX(-PI/90);
  //Z轴伸上来
  Z_toTop();
  Motor_middle.jxb_zhuandongX(-7*PI/180);
  Motor_bottom.jxb_zhuandongX(PI/90);
  //把块往转盘上放
  Motor_bottom.jxb_zhuandongX(-PI/2);
  line();
  //Z轴放到小转盘上
  Z_toZhuanPan();
  //爪子松开
  zhuazi_songkai();
  //Z轴伸上来
  initialize();

}

void send_command_signal(){
  digitalWrite(output,HIGH);
  delay(600);
  digitalWrite(output,LOW);
}

void grab_ace(){
  Motor_middle.jxb_zhuandongX(3*PI/180);
  Motor_bottom.jxb_zhuandongX(-3*PI/180);
  //Z轴伸下去,爪子夹住
  Z_down_zhuazi_jiazhu();
  //Z轴伸上来
  Z_toTop();
  //通知
  send_command_signal();
  
  Motor_middle.jxb_zhuandongX(-3*PI/180);
  Motor_bottom.jxb_zhuandongX(3*PI/180);
  //把块往转盘上放
  Motor_bottom.jxb_zhuandongX(-PI/2);
  line_ace();
  //Z轴放到小转盘上
  Z_toZhuanPan();
  
  //爪子松开
  zhuazi_songkai();
  
  //Z轴伸上来
  initialize();

}
void grab_zuihou(){
  Motor_middle.jxb_zhuandongX(3*PI/180);
  Motor_bottom.jxb_zhuandongX(-3*PI/180);
  //Z轴伸下去,爪子夹住
  Z_down_zhuazi_jiazhu();
  //Z轴伸上来
  Z_toTop();
  
  Motor_middle.jxb_zhuandongX(-3*PI/180);
  Motor_bottom.jxb_zhuandongX(3*PI/180);
  //把块往转盘上放
  Motor_bottom.jxb_zhuandongX(-PI/2);
  line_ace();
  //Z轴放到小转盘上
  Z_toZhuanPan();
  
  //爪子松开
  zhuazi_songkai();
  
  //Z轴伸上来
  initialize();

}

//0绿1白2红3黑4蓝
void send_color_data(int color){

  switch (color)
  {
  case 0:
    digitalWrite(output1,HIGH);
    break;
  case 1:
    digitalWrite(output2,HIGH);
    digitalWrite(output1,HIGH);
    break;
  case 2:
    digitalWrite(output2,HIGH);
    break;
  case 3:
    digitalWrite(output2,HIGH);
    digitalWrite(output3,HIGH);
    break;
  case 4:
    digitalWrite(output3,HIGH);
    break;  
  }
  delay(500);
  digitalWrite(output1,LOW);
  digitalWrite(output2,LOW);
  digitalWrite(output3,LOW);
}

void chishi(){

}

void yanseshibie(){
      digitalWrite(S2,LOW);
      digitalWrite(S3,LOW);
      colorBase[0] = pulseIn(OUT, LOW);
      Serial.print("R= ");
      Serial.print(colorBase[0]);
      Serial.print("  ");
      delay(100);
      digitalWrite(S2,HIGH);
      digitalWrite(S3,HIGH);
      colorBase[1] = pulseIn(OUT, LOW);
      Serial.print("G= ");
      Serial.print(colorBase[1]);
      Serial.print("  ");
      delay(100);
      digitalWrite(S2,LOW);
      digitalWrite(S3,HIGH);
      colorBase[2] = pulseIn(OUT, LOW);
      Serial.print("B= ");
      Serial.print(colorBase[2]);
      Serial.print("  ");
      delay(100);
      if (colorBase[0] < 50 && colorBase[1] < 50 && colorBase[2] < 50) {
        Serial.println("白色");
      }
      else if(colorBase[0] > 100 && colorBase[1] > 175 && colorBase[2] > 160) {
      Serial.println("黑色");
      }
      else {
        if (colorBase[0] < colorBase[1] && colorBase[0] < colorBase[2]) {
          Serial.println("红色");
        }
        else if (colorBase[0] > colorBase [1] && colorBase[1] < colorBase[2]){
          Serial.println("绿色");
        }
        else if (colorBase[0] > colorBase[2] && colorBase[1] > colorBase[2]){
          Serial.println("蓝色");
        }
      }
}


void setup() {
//设置减速比
  stepperHighZP.setReductionRatio(132.0 / 22.0, 200 * 16);  //big gear: 32, small gear: 9, steps per rev: 200, microsteps: 16
  stepperLowZP.setReductionRatio( 3.0 / 1.0, 200 * 16);
  Motor_bottom.setReductionRatio( 3.0 / 1.0, 200 * 16);
  Motor_middle.setReductionRatio( 3.0 / 1.0, 200 * 16);
  Motor_top.setReductionRatio( 1.0 / 1.0, 200 * 16);
  pinMode(weidong_1,INPUT);
  pinMode(weidong_2,INPUT);
  pinMode(weidong_3,INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(input,INPUT);
  pinMode(output1,OUTPUT);
  pinMode(output2,OUTPUT);
  pinMode(output3,OUTPUT);
  pinMode(output,OUTPUT);
  //小转盘转指定角度
  stepperHighZP.xiaozhuanpan_xdu(33*PI/180);
  //机械臂三轴归位
  initialize();
  digitalWrite(output,LOW);
  digitalWrite(output1,LOW);
  digitalWrite(output2,LOW);
  digitalWrite(output3,LOW);
  //颜色识别
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,LOW);
  Serial.begin(9600);
  //爪子复位
  zhuazi_setup();
  for (int i = 0; i < 5; i++)
  {
    colorBase[i] = -1;
  }
}

void loop() {
  //开机两个转盘归位，小转盘对准洞口，大转盘对准绿色
  delay(500);

  //等待，吃ACE
  while (1)
  {
    if(digitalRead(input) == HIGH){
      switch (i)
      {
      case 0: //A点
        initialize();
        grab_ace();
        stepperHighZP.xiaozhuanpan_xdu(-153*PI/180);
        yanseshibie_check();
        colorBase[0] = colorData;
        stepperHighZP.xiaozhuanpan_xdu(-24*PI/180);
        //通知
        digitalWrite(output,HIGH);
        send_color_data(colorBase[0]);
        digitalWrite(output,LOW);
        delay(200);
        //抖
        stepperHighZP.xiaozhuanpan_dou();
        delay(300);
        stepperHighZP.xiaozhuanpan_xdu(33*PI/180);
        
        //此处判断颜色是否为绿色，如果是等待高电平
        if (colorBase[0] == 0){
          lashi(0);
        }
        break;
      case 1: //C点
        initialize();
        grab_ace();
        stepperHighZP.xiaozhuanpan_xdu(-72*PI/180);
        if(colorBase[0] == 2){
          lashi(2);
        }
        break;
      case 2: //E点
        initialize();
        zhuazi_setup();
        grab_ace();
        stepperHighZP.xiaozhuanpan_xdu(-81*PI/180);
        if (colorBase[0] == 4)
        {
          lashi(4);
        }
        break;
      }
        i++;
      if (i == 3 && colorBase[0] == 1)
      {
        lashi(1);
        break;
      }
      else if(i == 3 && colorBase[0] == 3){
        lashi(3);
        break;
      }
      if(i == 3 && (colorBase[0] == 0 || colorBase[0] == 2 || colorBase[0] == 4)){
        break;
      }
    }
  }
  //此处吃完ACE且拉完
  //识别C

  yanseshibie_check();
  colorBase[2] = colorData;
  stepperHighZP.xiaozhuanpan_xdu(-24*PI/180);
  delay(200);
  //抖
  stepperHighZP.xiaozhuanpan_dou();
  delay(300);
  stepperHighZP.xiaozhuanpan_xdu(-48*PI/180);
  //识别E
  yanseshibie_check();
  colorBase[4] = colorData;
  stepperHighZP.xiaozhuanpan_xdu(-24*PI/180);
  delay(200);
  //抖
  stepperHighZP.xiaozhuanpan_dou();
  delay(300);
  i = 0;

  // F点
  zhuazi_setup();
  stepperHighZP.xiaozhuanpan_xdu(28*PI/180);
  while (1)
  {
    if (digitalRead(input) == HIGH){
      initialize();
      grab();
      delay(500);
      send_command_signal();
      //B点的块先到颜色识别
      yanseshibie_check();
      colorBase[1] = colorData;
      stepperHighZP.xiaozhuanpan_xdu(-28*PI/180);
      delay(300);
      //抖
      stepperHighZP.xiaozhuanpan_dou();
      delay(400);
      //C点的块不用颜色识别，直接进C颜色的大转盘
      stepperLowZP.dazhuanpan_terminal(password_in(colorBase[2]));
      stepperHighZP.xiaozhuanpan_xdu(-xiaozhuanpan_one);
      delay(300);
      //抖
      stepperHighZP.xiaozhuanpan_dou();
      delay(400);
      //D点的块先颜色识别
      stepperHighZP.xiaozhuanpan_xdu(-44*PI/180);
      yanseshibie_check();
      colorBase[3] = colorData;
      stepperHighZP.xiaozhuanpan_xdu(-28*PI/180);
      delay(300);
      //抖
      stepperHighZP.xiaozhuanpan_dou();
      delay(400);
      //E点的块不用颜色识别，直接进E颜色的大转盘
      stepperLowZP.dazhuanpan_terminal(password_in(colorBase[4]));
      stepperHighZP.xiaozhuanpan_xdu(-xiaozhuanpan_one);
      delay(300);
      //抖
      stepperHighZP.xiaozhuanpan_dou();
      delay(400);
      //A点的块不用颜色识别，直接进A颜色的大转盘
      stepperLowZP.dazhuanpan_terminal(password_in(colorBase[0]));
      //让车从F开始走到G
      stepperHighZP.setCurrentInitialPosition();
      stepperHighZP.xiaozhuanpan_xdu(-xiaozhuanpan_one);
      delay(300);
      //抖
      stepperHighZP.xiaozhuanpan_dou();
      delay(400);
      break;
    }
  }

  send_command_signal();
  
  //G
  stepperHighZP.xiaozhuanpan_xdu(28*PI/180);
  while (1)
  {
    if (digitalRead(input) == HIGH){
      initialize();
      grab();
      delay(500);
      //A点的块进A颜色的大转盘
      stepperLowZP.dazhuanpan_terminal(password_in(colorBase[0]));
      stepperHighZP.xiaozhuanpan_xdu(-28*PI/180);
      delay(400);
      //抖
      stepperHighZP.xiaozhuanpan_dou();
      delay(400);
      //E点的块进E颜色的大转盘
      stepperLowZP.dazhuanpan_terminal(password_in(colorBase[4]));
      stepperHighZP.xiaozhuanpan_xdu(-xiaozhuanpan_one);
      delay(400);
      //抖
      stepperHighZP.xiaozhuanpan_dou();
      delay(400);
      //D点的块进D颜色的大转盘
      stepperLowZP.dazhuanpan_terminal(password_in(colorBase[3]));
      stepperHighZP.xiaozhuanpan_xdu(-xiaozhuanpan_one);
      delay(400);
      //抖
      stepperHighZP.xiaozhuanpan_dou();
      delay(400);
      //C点的块进C颜色的大转盘
      stepperLowZP.dazhuanpan_terminal(password_in(colorBase[2]));
      stepperHighZP.setCurrentInitialPosition();
      stepperHighZP.xiaozhuanpan_xdu(-xiaozhuanpan_one);
      delay(400);
      //抖
      stepperHighZP.xiaozhuanpan_dou();
      delay(400);
      //B点的块进B颜色的大转盘
      stepperLowZP.dazhuanpan_terminal(password_in(colorBase[1]));
      send_command_signal();
      stepperHighZP.xiaozhuanpan_xdu(-xiaozhuanpan_one);
      delay(400);
      //抖
      stepperHighZP.xiaozhuanpan_dou();
      delay(400);
      break;
    }
  }
  
  send_command_signal();
  int count = 4;
  while (1)
  {
    if(digitalRead(input) == HIGH){
      if(count == colorBase[0]){
        stepperHighZP.xiaozhuanpan_xdu(33*PI/180);
        initialize();
        grab_zuihou();
        stepperLowZP.dazhuanpan_terminal(password_in(colorBase[0]));
        stepperHighZP.xiaozhuanpan_xdu(-177*PI/180);
        //抖

        stepperHighZP.xiaozhuanpan_dou();
        //delay(400);
        send_command_signal();
      }
      lashi(count);
      count--;
      if (count < 0)
      {
        break;
      }

    }
  }

//胜利大逃亡
      digitalWrite(output,HIGH);
      delay(300);
      digitalWrite(output,LOW);
  while (1)
  {
    /* code */
  }
  
  
}