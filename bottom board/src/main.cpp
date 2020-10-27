//1black
//90du 9000
#include <Arduino.h>
#include<pinout.h>
#include<RampsStepper.h>
#include<Stepper.h>

//A的颜色
int colorA=0;


//巡线步进电机转呀转
void cmdGripperOn(int step);
//电机正反转
void leftON();
void rightON();
void leftOFF();
void rightOFF();
void xunxian();
void xunxianC();   //300次数
//原地校准
void jiaozhun();
void xian(int num); //看几次黑线
void quxian();//看曲线
void quhei(int num);//曲线巡线看黑线
void turnLeft(int time);
void turnRight(int time);
void layi();
void G();

void leftON(int time);
void rightON(int time);
//左侧电机fan转调速
void leftOFF(int time);
//you侧电机fan转调速
void rightOFF(int time);
//展开看线数
void xianshu(int num);
//ace
void xianshu1(int num);
//前进后退
void back(int num);
void ahead(int num);
//从起点走到心
void xin();
//从中心走到色块
void se();
//从中心走到A、C、E
void ACE();
void FG();
//往左转
void leftqu(int num);
//从F到G骚走线
void sao();
//最后放料拉屎
void lashi();
//拉屎左边看黑线
void quzuohei(int num);
//最后五个块寻大圈
void qulashi(int num);
//如果为红或蓝，拿完C或E直接去拉A
void CEbreak();
//从识别块得到颜色，编号，为规划路径做准备
int A();

//从G到蓝色区域
void toblue();
//绕开白点走小曲线
void quhei1(int num);
//F to G
void quhei2(int num);
//拉最后四个
void lasige(int se);

//寻线电机
Stepper stepper(2400, STEPPER_GRIPPER_PIN_0, STEPPER_GRIPPER_PIN_1, STEPPER_GRIPPER_PIN_2, STEPPER_GRIPPER_PIN_3);
//电机
RampsStepper rightStepper(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN);
RampsStepper leftStepper(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN);


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(input1,INPUT);
  pinMode(input2,INPUT);
  pinMode(input3,INPUT);
  pinMode(input,INPUT);
  pinMode(output,OUTPUT);
  digitalWrite(output,LOW);
  Serial.println("0");
  //左右巡线灰度
  pinMode(X_MAX_PIN,INPUT);      //右  
  pinMode(X_MIN_PIN,INPUT);      //左 
  
  //两板通讯
}

void loop() {
   
/*从起点走到心*/
  xin();
/*A*/
  cmdGripperOn(60);
  turnLeft(9300);
  ACE();
  turnRight(9400);

  while (1)
  {
    if(digitalRead(input)==HIGH)
    {
      delay(100);
      A();
      Serial.print("颜色：");
      Serial.println(colorA);
      break;      
    }
  }
  //判断一波A的颜色是不是绿
  if(colorA==1)
  {
    turnLeft(9400);
    layi();
    turnLeft(9400);
  }
/*C*/
  if(colorA==3)  //红色
  {
    CEbreak();
    turnLeft(9400);
  }
  else
  {
    ACE();
    turnRight(9400);
  } 
/*E*/
  if(colorA==5)  //蓝色
  {
    CEbreak();
    turnRight(9400);
  }
  else
  {
   ACE();
   turnLeft(9400);
  }
/*目前车头朝着C，看一下是白是黑*/
if(colorA==2)
{/*白*/
  turnLeft(4600);
  layi();
  turnRight(9400);
  //等待CE进肚
  
}
else if(colorA==4)
{/*黑*/
  turnRight(4700);
  layi();
}
else
 {
   turnLeft(14000);
 }
  

/*F*/
 
   FG();
  digitalWrite(output,HIGH);
  delay(1500);
  digitalWrite(output,LOW);
  while(1)
  {
    if(digitalRead(input)==HIGH)
      break;
  }
   sao();

/*G*/
   G();
toblue();
for(int i=4;i>0;i--)
{
  lasige(i);
}

while(1)
{
}

}
void cmdGripperOn(int step) {
  stepper.setSpeed(10);
  stepper.step(step);
  delay(50);
  digitalWrite(STEPPER_GRIPPER_PIN_0, LOW);
  digitalWrite(STEPPER_GRIPPER_PIN_1, LOW);
  digitalWrite(STEPPER_GRIPPER_PIN_2, LOW);
  digitalWrite(STEPPER_GRIPPER_PIN_3, LOW);
  //printComment("// NOT IMPLEMENTED");
  //printFault();
}
void xunxian()
{
  if((digitalRead(X_MAX_PIN)==1)&&(digitalRead(X_MIN_PIN)==0))
  {
    while((digitalRead(X_MAX_PIN)==1)&&(digitalRead(X_MIN_PIN)==0))
    {
      leftON(50);
      rightOFF(50);
    }
  }
  else if((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==1))
  {
    while((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==1))
    {
      leftOFF(50);
      rightON(50);
      

    }

  }
  else if((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==0))
  {
    while ((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==0))
    {
      leftON();
      rightON();
    }
  }

}
//限定次数
void xunxianC()
{
  if((digitalRead(X_MAX_PIN)==1)&&(digitalRead(X_MIN_PIN)==0))
  {
    for(int i=0;i<100;i++)
    {
      leftON(50);
      
      rightOFF(50);

    }
  }
  else if((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==1))
  {
    for(int i=0;i<100;i++)
    {
      leftOFF(50);
      
      rightON(50);
    }

  }
  else if((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==0))
  {
    for(int i=0;i<100;i++)
    {
      leftON();
      rightON();
    }
  }
}  
//左侧电机正转
void leftON()
{
  
  
    digitalWrite(Y_DIR_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(20);
}

//右侧电机正转
void rightON()
{
    digitalWrite(X_DIR_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(X_STEP_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(X_STEP_PIN, LOW);
    delayMicroseconds(20);
}

//左侧电机反转
void leftOFF()
{
    digitalWrite(Y_DIR_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(20);
}

//右侧电机反转
void rightOFF()
{
    digitalWrite(X_DIR_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(X_STEP_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(X_STEP_PIN, LOW);
    delayMicroseconds(20);
}

//左侧电机正转调速
void leftON(int time)
{
 
    digitalWrite(Y_DIR_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(time);
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(time); 
   
}

//右侧电机调速
void rightON(int time)
{
    digitalWrite(X_DIR_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(X_STEP_PIN, HIGH);
    delayMicroseconds(time);
    digitalWrite(X_STEP_PIN, LOW);
    delayMicroseconds(time);
}
//左侧电机fan转调速
void leftOFF(int time)
{
 
    digitalWrite(Y_DIR_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(time);
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(time); 
   
}
//右侧电机反转tiaosu
void rightOFF(int time)
{
    digitalWrite(X_DIR_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(X_STEP_PIN, HIGH);
    delayMicroseconds(time);
    digitalWrite(X_STEP_PIN, LOW);
    delayMicroseconds(time);
}
//转右
void turnRight(int time)
{
  for(int i=0;i<time;i++)
  {
    leftON();
    rightOFF();
  }
}
//左转
void turnLeft(int time)
{
  for(int i=0;i<=time;i++)
  {
    leftOFF();
    rightON();
  }
}
//夹紧巡线
void xian(int num)
{
   int count=0;
   while(count!=num)
  {
    if((digitalRead(X_MAX_PIN)==1)&&(digitalRead(X_MIN_PIN)==1))
   {
     count++;
   } 
    xunxian();
  }
  ahead(2000);
  
  
}

//展开看线
void xianshu(int num)
{
   int count=0;
   while(count!=num)
  {
    //只要有传感器看见就加一
    if((digitalRead(X_MAX_PIN)==1)||(digitalRead(X_MIN_PIN)==1))
   {
      count++;
     for(int i=0;i<500;i++)
     {
       leftON();
       rightON();
     }

   }
    for(int i=0;i<800;i++)
    {
      leftON();
      rightON();
    }
  }
  
}
//A点
void xianshu1(int num)
{
   int count=0;
   while(count!=num)
  {
    //只要有传感器看见就加一
    if((digitalRead(X_MAX_PIN)==1))
   {
      count++;
     for(int i=0;i<500;i++)
     {
       leftON();
       rightON();
     }

   }
    for(int i=0;i<500;i++)
    {
      leftON();
      rightON();
    }
  }
  
}
//前进
void ahead(int num)
{
  float half=num/2;
  int speed = 200;
  int step=0;
  for (int  i = 0; i <half; i++)
  {
    speed=speed-0.5;
    if(speed<=20){
      speed=20;
      step++;
    }
    leftON(speed);
    rightON(speed);
  }
  for(int i=0;i<step;i++)
  {
    leftON(speed);
    rightON(speed);
  }
  for (int  i = 0; i <half-step; i++)
  {
    
    speed=speed+0.5;
    if(speed>=200){
      speed=200;
    }
    leftON(speed);
    rightON(speed);

  }
}
//后退
void back(int num)
{ 
  float half=num/2;
  int speed = 200;
  int step=0;
  for (int  i = 0; i <half; i++)
  {
    speed=speed-0.5;
    if(speed<=20){
      speed=20;
      step++;
    }
    leftOFF(speed);
    rightOFF(speed);
  }
  for(int i=0;i<step;i++)
  {
    leftOFF(speed);
    rightOFF(speed);
  }
  for (int  i = 0; i <half-step; i++)
  {
    
    speed=speed+0.5;
    if(speed>=200){
      speed=200;
    }
    leftOFF(speed);
    rightOFF(speed);

  }
}

//从中心走到色块
void se()
{
  cmdGripperOn(-310);
  for(int i=0;i<50;i++)
  xunxianC();
  delay(2000);
  cmdGripperOn(320);
  xianshu(2);
  delay(500);
}

//起点走到心
void xin()
{
  //转传感器夹紧
  cmdGripperOn(40);
  cmdGripperOn(-270);
  xianshu(1);
  ahead(8000);
  xian(1);
  cmdGripperOn(-25);
  for(int i=0;i<160;i++)
    xunxianC();
  cmdGripperOn(330);
  xianshu(1);    
  ahead(11000);
}
//从中心走到A、C、E
void ACE()
{
  //夹紧巡线
  cmdGripperOn(-295);
  delay(200);
  for(int i=0;i<80;i++)
    jiaozhun();
  //调整车位置
  for(int i=0;i<150;i++)
    xunxianC();
  delay(100);
  for(int i=0;i<100;i++)
    jiaozhun();
  //放开到点
  cmdGripperOn(320);
  xianshu1(1);
  digitalWrite(output,HIGH);
  
  Serial.println("高电平：");
  Serial.println("1");         //给转盘的板子通电
  
  
  //夹住后退
  cmdGripperOn(300);
  cmdGripperOn(-200);
for(int i=0;i<1500;i++)
{
  leftOFF(100);
  rightOFF(100);
}
digitalWrite(output,LOW);
cmdGripperOn(310);
  while(1){
    if(digitalRead(input)==HIGH){
    Serial.println("收到");
         break;
   }
  }
  back(16300);
}
//吃FG
void FG()
{
  cmdGripperOn(-295);
  xian(1);
  for(int i=0;i<80;i++)
    xunxianC();
  cmdGripperOn(350);
  xianshu(1);
  for(int i=0;i<600;i++)
  {
   leftON(100);
   rightON(100);
  }
  cmdGripperOn(-295);
  for(int i=0;i<60;i++)
    jiaozhun();
  for(int i=0;i<40;i++)
    xunxianC();
  cmdGripperOn(350);
  xianshu(1);
  for (int  i = 0; i < 4500; i++)
  {
    leftON(100);
    rightON(100);
  }
  

}
//F到G寻小曲线
void quxian()
{
  if((digitalRead(X_MAX_PIN)==1)&&(digitalRead(X_MIN_PIN)==0))
  {
    while((digitalRead(X_MAX_PIN)==1)&&(digitalRead(X_MIN_PIN)==0))
    {
      leftON();
      rightON();

    }
  }
  else if((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==1))
  {
    while((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==1))
    {
      leftOFF();
      rightON();
      rightON();
    }

  }
  else if((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==0))
  {
    while ((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==0))
    {
      leftON();
      leftON();
      rightON();
      rightON();
      rightON();
    }
  }
}
//从F到G骚巡线看黑线
void quhei(int num)
{
   int count=0;
   while(count!=num)
  {
    if((digitalRead(X_MAX_PIN)==1)&&(digitalRead(X_MIN_PIN)==1))
   {
     count++;
   }
    quxian();
    ahead(200);
  }
  leftqu(2000);
}
//往左转
void leftqu(int num)
{
  for(int i=0;i<=num;i++)
  {
    leftON();
    leftON();
    rightON();
    rightON();
    rightON();
  }
}
//从F到G骚走线
void sao()
{
  back(23000);
  /*寻曲线*/
  turnLeft(10200);
  while(1)
  {
    if(digitalRead(input)==HIGH)
    break;
  }
  cmdGripperOn(-10);
  quhei2(1);
  quhei2(1);
  ahead(1900);
  turnRight(8000);
  cmdGripperOn(-285);
  for(int i=0;i<80;i++)
    xunxianC();
}

//走G点
void G()
{
  cmdGripperOn(320);
  xianshu(1);
  ahead(1000);
  cmdGripperOn(-297);
  for(int i=0;i<60;i++)
    jiaozhun();
  for(int i=0;i<40;i++)
    xunxianC();
  cmdGripperOn(330);
  xianshu(1);
  for(int i=0;i<2000;i++)
  {
    leftON(100);
    rightON(100);
  }
  digitalWrite(output,HIGH);
  delay(200);
  digitalWrite(output,LOW);
  while(1)
  {
    if(digitalRead(input)==HIGH)
    break;
  }
 }
void lashi()
{
  if((digitalRead(X_MAX_PIN)==1)&&(digitalRead(X_MIN_PIN)==0))
  {
    for(int i=0;i<500;i++)
    {
      leftON(50);
      rightON(50);
    }
  }
 
  else if((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==0))
  {
    for(int i=0;i<50;i++)
    {
      leftON(50);
      leftON(50);
      leftON(50);
      leftON(50);
      rightON(50);
      rightON(50);
      rightON(50);
      rightON(50);
      rightON(50);
    }
  }
}
//最后五个块寻大圈
void qulashi(int num)
{

   int count=0;
   while(count!=num)
  {
    if((digitalRead(X_MAX_PIN)==1)&&(digitalRead(X_MIN_PIN)==1))
   {
     count++;
   }
    lashi();
    ahead(200);
  }
  leftqu(2500);
}
//放A一个快
void layi()
{
  cmdGripperOn(40);
  cmdGripperOn(-295);
  for(int i=0;i<=110;i++)
    xunxianC();
  cmdGripperOn(320);
  xianshu(1);
  ahead(1000);
  cmdGripperOn(-295);
  for(int i=0;i<=150;i++)
    xunxianC();
  cmdGripperOn(320);
  //到放块的圈
  xianshu(1);
  delay(20);
  turnLeft(18300);
  delay(100);
  cmdGripperOn(-295);
  for(int i=0;i<=60;i++)
    jiaozhun();
  ahead(1000);
  for(int i=0;i<=60;i++)
    jiaozhun();
  ahead(1500);
  
  digitalWrite(output,HIGH);
  delay(300);
  digitalWrite(output,LOW);
  while(1){
    if(digitalRead(input)==HIGH){
         break;
   }
  
  }
  /*回程*/
  ahead(2500);
  for(int i=0;i<=60;i++)
    xunxianC();
  cmdGripperOn(320);
  xianshu(1);
  cmdGripperOn(-295);
  for(int i=0;i<=60;i++)
    xunxianC();
  cmdGripperOn(330);
  xianshu(1);    
  ahead(11000);

}

//规划拉A的路径
int A()
{
  if(digitalRead(input2) == HIGH)
  {
    if(digitalRead(input1) == HIGH)
    {
      colorA=2;
    }
    else if(digitalRead(input3) == HIGH)
    {
     colorA=4;
    }
    else
    {
      colorA=3;
    }
          
  }
  else
  {
    if(digitalRead(input1) ==HIGH){
    colorA=1;
    }
    else if(digitalRead(input3) == HIGH){
       colorA = 5;
    }
  }
  return colorA;
} 
//如果为红或蓝
void CEbreak()
{
  //夹紧巡线
  cmdGripperOn(-295);
  //调整车位置
  for(int i=0;i<150;i++)
    xunxianC();
  delay(100);
  //放开到点
  cmdGripperOn(320);
  xianshu1(1);
  delay(100);
  //夹住后退
  cmdGripperOn(300);
  cmdGripperOn(-200);
  digitalWrite(output,HIGH);
  delay(100);
  Serial.println("1");         //给转盘的板子通电
  digitalWrite(output,LOW);
for(int i=0;i<1500;i++)
{
  leftOFF(100);
  rightOFF(100);
}
 cmdGripperOn(310);
  
  while(1){
    if(digitalRead(input)==HIGH){
         break;    
    }
  }
  ahead(2500);
  //夹紧巡线
  cmdGripperOn(-295);
  for(int i=0;i<=140;i++)
    xunxianC();
  //过白点点以后原地校准
  for(int i=0;i<=60;i++)
    jiaozhun();
  cmdGripperOn(320);
  //到放块的圈
  xianshu(1);
  delay(100);
  turnLeft(18600);
  delay(100);
  cmdGripperOn(-295);
  //物块原地矫正,前进到指定区域
  for(int i=0;i<=60;i++)
    jiaozhun();
  ahead(1000);
  for(int i=0;i<=60;i++)
    jiaozhun();
  ahead(1500);
  digitalWrite(output,HIGH);
  delay(500);
  digitalWrite(output,LOW);
  while(1){
    if(digitalRead(input)==HIGH){
         break;
   }
  
  }
  /*回程*/
  ahead(3000);
  for(int i=0;i<=60;i++)
    jiaozhun();
  cmdGripperOn(320);
  xianshu(1);
  //可能会识别到白圈旁边的黑线多走一点
  ahead(2500);
  cmdGripperOn(-295);
  for(int i=0;i<=110;i++)
    xunxianC();
  cmdGripperOn(330);
  xianshu(1);    
  ahead(11000);
}

//原地校准
void jiaozhun()
{
   if((digitalRead(X_MAX_PIN)==1)&&(digitalRead(X_MIN_PIN)==0))
  {
    for(int i=0;i<100;i++)
    {
      leftON(20);
      rightOFF(50);

    }
  }
  else if((digitalRead(X_MAX_PIN)==0)&&(digitalRead(X_MIN_PIN)==1))
  {
    for(int i=0;i<100;i++)
    {
      leftOFF(50);
      rightON(20);
    }

  }
}

void quzuohei(int num)
{
  cmdGripperOn(50);
  cmdGripperOn(-200);
  int count=0;
   while(count!=num)
  {
    //只要有传感器看见就加一
    if(digitalRead(X_MIN_PIN)==1)
   {
      count++; 
   }
   lashi();
    Serial.println(count);
  }
   Serial.println(count); 
     leftqu(1000);
}

void toblue()
{
  back(21000);
  /*寻曲线*/
  turnLeft(9800);
 
  cmdGripperOn(-200);
  quhei1(1);
  
  ahead(1000);
  turnRight(8300);
  cmdGripperOn(-105);
  for(int i=0;i<80;i++)
  jiaozhun();
  for(int i=0;i<180;i++)
    xunxianC();
  for(int i=0;i<80;i++)
  jiaozhun();
  //张开到点
  cmdGripperOn(320);
  xianshu1(1);
  delay(100);
  if(colorA==5)
  {

    digitalWrite(output,HIGH);
    delay(500);
    digitalWrite(output,LOW);
    //夹住后退
   
   cmdGripperOn(-210);
   for(int i=0;i<1500;i++)
   {
     leftOFF(100);
     rightOFF(100);
   }
   cmdGripperOn(300);
   while(1)
   {
     if(digitalRead(input)==HIGH)
     {
       break;
     }
   }
   for(int i=0;i<1500;i++)
   {
     leftON(100);
     rightON(100);
   }
  }

  turnLeft(18600);
  delay(100);
  cmdGripperOn(-290);
  //物块原地矫正,前进到指定区域
  for(int i=0;i<=60;i++)
    jiaozhun();
  ahead(800);
  for(int i=0;i<=60;i++)
    jiaozhun();
  ahead(1300);
  for(int i=0;i<=60;i++)
    jiaozhun();
    delay(200);
  digitalWrite(output,HIGH);
  delay(1000);
  digitalWrite(output,LOW);
  while(1){
    if(digitalRead(input)==HIGH){
         break;
   }
  
  }
  for(int i=0;i<1500;i++)
  {
    leftON(100);
    rightON(100);
  }
  /*回程*/
  for(int i=0;i<=60;i++)
    xunxianC();
  cmdGripperOn(320);
  xianshu(1);  
  ahead(8000);
}
//绕过白点
void quhei1(int num)
{
   int count=0;
   while(count!=num)
  {
    if(digitalRead(X_MIN_PIN)==1)
   {
     count++;
   }
    for(int i=0;i<100;i++)
    {
      leftON();
      leftON();
      rightON();
      rightON();
      rightON();
    }
    
  }
  leftqu(2500);
}
void quhei2(int num)
{
   int count=0;
   while(count!=num)
  {
    if((digitalRead(X_MIN_PIN)==1)||(digitalRead(X_MAX_PIN)==1))
   {
     count++;
   }
    for(int i=0;i<100;i++)
    {
      leftON();
      leftON();
      rightON();
      rightON();
      rightON();
    }
    
  }
  leftqu(2500);
}

void lasige(int se)
{
  turnRight(8500);
  cmdGripperOn(-200);
  quhei1(1);
  
  ahead(1000);
  turnRight(8300);
  cmdGripperOn(-90);
  for(int i=0;i<50;i++)
  jiaozhun();
  for(int i=0;i<180;i++)
    xunxianC();
  for(int i=0;i<100;i++)
    jiaozhun();
  //张开到点
  cmdGripperOn(390);
  xianshu1(1);
  delay(100);
  if(colorA==se)
  {
    digitalWrite(output,HIGH);
    delay(200);
    digitalWrite(output,LOW);
    //夹住后退
   
   cmdGripperOn(-200);
   for(int i=0;i<2500;i++)
   {
     leftOFF(100);
     rightOFF(100);
   }
   cmdGripperOn(360);
   while(1)
   {
     if(digitalRead(input)==HIGH)
     {
       break;
     }
   }
   for(int i=0;i<2300;i++)
   {
     leftON(100);
     rightON(100);
   }
  }
    
  turnLeft(18600);
  delay(100);
  cmdGripperOn(-290);
  //物块原地矫正,前进到指定区域
  for(int i=0;i<=60;i++)
    jiaozhun();
  ahead(900);
  for(int i=0;i<=60;i++)
    jiaozhun();
  ahead(1100);
  for(int i=0;i<200;i++)
  {
    leftON(100);
    rightON(100);
  }
  for(int i=0;i<=60;i++)
    jiaozhun();
    delay(200);
  digitalWrite(output,HIGH);
  delay(100);
  digitalWrite(output,LOW);
  while(1){
    if(digitalRead(input)==HIGH){
         break;
   }
  }
  for(int i=0;i<1500;i++)
   {
     leftON(100);
     rightON(100);
   }
  
  /*回程*/
  if(se==1)
  {

   for(int i=0;i<4000;i++)
   {
     leftON(100);
     rightON(100);
   }

   turnRight(4700);
   for(int i=0;i<30000;i++)
   {
     leftON();
     rightON();
   }
   for(int i=0;i<20000;i++)
   {
     leftON();
     rightON();
   }
   while(1)
   {
     
   }
  }
 for(int i=0;i<=60;i++)
   xunxianC();
   cmdGripperOn(320);
   xianshu(1);  
   ahead(8000);
  
  
 

}