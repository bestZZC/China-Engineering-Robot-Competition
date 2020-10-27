#include <Arduino.h>

/*     Arduino Color Sensing Tutorial
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
int colorBase[3];
#define S0 41
#define S1 39
#define S2 2
#define S3 14
#define OUT 3
#define LED 43
#define YANSE_SIGNAL 9     //绿 9
#define YANSE_D1 10         //蓝 10
#define YANSE_D2 11         //黑 11
#define YANSE_D3 12         //红 12

int frequency = 0;
void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  pinMode(LED,OUTPUT);
  pinMode(YANSE_SIGNAL,INPUT);
  pinMode(YANSE_D1,OUTPUT);
  pinMode(YANSE_D2,OUTPUT);
  pinMode(YANSE_D3,OUTPUT);
  digitalWrite(LED,HIGH);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,LOW);
  Serial.begin(9600);
}
void loop() {
  while (1)
  {
    if (digitalRead(YANSE_SIGNAL) == LOW)
    {
      digitalWrite(YANSE_D1,LOW);
      digitalWrite(YANSE_D2,LOW);
      digitalWrite(YANSE_D3,LOW);
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
      if (colorBase[0] < 150 && colorBase[1] < 150 && colorBase[2] < 150) {
        Serial.println("白色");
        digitalWrite(YANSE_D2,HIGH);
        digitalWrite(YANSE_D3,HIGH);
      }
      else if(colorBase[0] > 175 && colorBase[1] > 175 && colorBase[2] > 175) {
      Serial.println("黑色");
      digitalWrite(YANSE_D1,HIGH);
      digitalWrite(YANSE_D2,HIGH);
      }
      else {
        if (colorBase[0] < colorBase[1] && colorBase[0] < colorBase[2]) {
          Serial.println("红色");
          digitalWrite(YANSE_D3,HIGH);
        }
        else if (colorBase[0] > colorBase [1] && colorBase[1] < colorBase[2]){
          Serial.println("绿色");
          digitalWrite(YANSE_D2,HIGH);
        }
        else if (colorBase[0] > colorBase[2] && colorBase[1] > colorBase[2]){
          Serial.println("蓝色");
          digitalWrite(YANSE_D1,HIGH);
        }
      }
    }
  }
}