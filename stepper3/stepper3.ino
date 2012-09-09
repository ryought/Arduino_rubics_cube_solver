
#define STEPS 72

#include <Servo.h>

int pin[4] = {10,11,12,13};

Servo servo;
int s_deg;
 
void forward(int rot){
  for(int l=0;l<rot*STEPS;l++){
    Serial.print(l);
    Serial.print(" : ");
    for(int i=0;i<4;i++){
      Serial.print(i);
       int prev = i-1;
       int next = i+1;
       if(prev < 0) prev = 3;
       if(next > 3) next = 0;
       digitalWrite(pin[prev], LOW);
       digitalWrite(pin[i], HIGH);
       digitalWrite(pin[next], HIGH);
       delay(8);
     }
     
   Serial.println("");
   }
   delay(1000);
}
void reverse(int rot){
  for(int i=0;i<rot*STEPS;i++){
     for(int i=3;i>=0;i--){
       int prev = i+1;
       int next = i-1;
       if(prev > 3) prev = 0;
       if(next < 0) next = 3;
       digitalWrite(pin[prev], LOW);
       digitalWrite(pin[i], HIGH);
       digitalWrite(pin[next], HIGH);
       delay(8);
     }
   }
   delay(1000);
}
 
void setup(){
  for(int i=0;i<4;i++){
    pinMode(pin[i], OUTPUT);
  }
  Serial.begin(9600);
  servo.attach(7);
}
 
void loop(){
  s_deg = 28;
  Serial.print("Servo : ");
  Serial.println(s_deg);
  servo.write(s_deg);
  Serial.println("forward 1");
  forward(1); // 1回正転
  
  s_deg = 110;
  Serial.print("Servo : ");
  Serial.println(s_deg);
  servo.write(s_deg);
  
  Serial.println("reverse 1");
  reverse(1); // 1回逆転
}
