#include <Servo.h>
#include <Wire.h>
#define Servo_one 11
#define Trig 8
#define Echo 9

#define Manual_b 3
#define Right_b 4
#define Left_b 5
#define Up_b 6
#define Down_b 7
int M_c = 0;
int R_c = 0;
int L_c = 0;
int U_c = 0;
int D_c = 0;
int ButtonDelay = 50;
int flag = 0;
int endflag = 0;
int mode = 0;

int cc = 5; //毎秒約5cc
int val = 100;
int loadtime = 0;
unsigned long time;

Servo servo1;

void setup() {
  // put your setup code here, to run once:
  servo1.attach(Servo_one);
  Serial.begin(57600);
  pinMode(Trig,OUTPUT);
  pinMode(Echo,INPUT);
  for(int i = 3; i <= 7; i++) pinMode(i, INPUT_PULLUP);
  
  //ディスプレイ初期化
  pinMode(A2, OUTPUT);
  digitalWrite(A2,HIGH);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  delay(500); //待機時間(必須)
  printScreen();
}

void loop() {
  if(mode != 2) RangeCheck();
  ButtonCheck();
  //printScreen();
}

void ButtonCheck() {
  //マニュアルボタン
  if(digitalRead(Manual_b) == LOW){
    if(M_c <= ButtonDelay) M_c++;
    if(M_c == ButtonDelay) {
      //押された時の処理
      Serial.println("Manual");
      while(digitalRead(Manual_b) == LOW) {
        servo1.writeMicroseconds(2200);
      }
      servo1.writeMicroseconds(0);
      //servo1.writeMicroseconds(2200);
    }
  } else {
    M_c = 0;
    //servo1.writeMicroseconds(0);
  }
  
  //右ボタン
  if(digitalRead(Right_b) == LOW){
    if(R_c <= ButtonDelay) R_c++;
    if(R_c == ButtonDelay) {
      //押された時の処理
      Serial.println("Right");
      if(mode < 2) mode++;
      if(mode == 2) {
        servo1.writeMicroseconds(0);
        flag = 0;
        endflag = 0;
      }
      printScreen();
    }
  } else {
    R_c = 0;
  }
  
  //左ボタン
  if(digitalRead(Left_b) == LOW){
    if(L_c <= ButtonDelay) L_c++;
    if(L_c == ButtonDelay) {
      //押された時の処理
      Serial.println("Left");
      if(mode > 0) mode--;
      printScreen();
    }
  } else {
    L_c = 0;
  }
  
  //上ボタン
  if(digitalRead(Up_b) == LOW){
    if(U_c <= ButtonDelay) U_c++;
    if(U_c == ButtonDelay) {
      //押された時の処理
      Serial.println("Up");
      if(mode == 0) {
        if(val < 500) val += 10;
      } else if(mode == 1){
        loadtime++;
      }
      printScreen();
    }
  } else {
    U_c = 0;
  }
  
  //下ボタン
  if(digitalRead(Down_b) == LOW){
    if(D_c <= ButtonDelay) D_c++;
    if(D_c == ButtonDelay) {
      //押された時の処理
      Serial.println("Down");
      if(mode == 0) {
        if(val > 50) val -= 10;
      } else if(mode == 1){
        if(loadtime > 0) loadtime--;
      }
      printScreen();
    }
  } else {
    D_c = 0;
  }
}

void printScreen() {
  char str[4];
  char str2[4];
  lcd_init();
  lcd_setCursor(0,0);
  if(flag == 1) {
    lcd_printStr("Wait...");
    lcd_setCursor(0,1);
    sprintf(str, "%3d", (int)((millis() - time) / 1000));
    lcd_printStr(str);
    lcd_printStr("/");
    sprintf(str2, "%3d", val / cc + loadtime);
    lcd_printStr(str2);
    return;
  }
  if(mode == 0) { 
    lcd_printStr("Serve");
    lcd_setCursor(2,1);
    sprintf(str, "%3d", val);
    lcd_printStr(str);
    lcd_printStr("cc");
  } else if(mode == 1){
    lcd_printStr("LoadTime");
    lcd_setCursor(2,1);
    sprintf(str, "%2d", loadtime);
    lcd_printStr(str);
    lcd_printStr("sec");
  } else {
    lcd_printStr("Manual");
    lcd_setCursor(2,3);
    lcd_printStr("Mode");
  }
}
