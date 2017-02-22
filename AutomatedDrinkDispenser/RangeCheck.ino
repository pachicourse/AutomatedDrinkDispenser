int Duration;
float Distance;
int Range_c = 0;

void RangeCheck() {
  digitalWrite(Trig,LOW);
  delayMicroseconds(1);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(11);
  digitalWrite(Trig,LOW);
  Duration = pulseIn(Echo,HIGH);
  if (Duration>0) {
    Distance = Duration/2;
    Distance = Distance*340*100/1000000; // ultrasonic speed is 340m/s = 34000cm/s = 0.034cm/us 
//    Serial.print(Duration);
//    Serial.print(" us ");
//    Serial.print(Distance);
//    Serial.println(" cm");
    if(Distance <= 6.5) {
      if(Range_c < 600) Range_c++;
      if(Range_c == 600) {
        //処理
        //Serial.println("GO!");
        if(endflag == 1) {
          return;
        }
        if(flag == 1 && (millis() - time) / 1000 >= (val / cc) + loadtime) {
          //Serial.println("end");
          endflag = 1;
          flag = 0;
          printScreen();
        }; 
        if(flag == 0 && endflag == 0) {
          time = millis();
          flag = 1;
          Serial.println("start");
          Serial.println(flag);
        }
        printScreen();
        //Serial.println("ranged");
      }
    } else {
      //Serial.println("out");
      if(flag == 1) {
        flag = 0;
        printScreen();
      }
      time = millis();
      endflag = 0;
      Range_c = 0;
    }
  }
  servo1.writeMicroseconds(flag == 1?2200:1);
  //delay(100);
}
