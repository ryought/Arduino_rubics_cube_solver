//#include <Servo.h>
const byte motorPen[]={0x03,0x06,0x0C,0x09}; //PortBの励磁パターン
const byte motorBall[]={0x90,0xC0,0x60,0x30}; //PortDの励磁パターン
//Servo servoPen;
 
void setup(){
  DDRB |=B00001111; //PortB 8,9,10,11
  DDRD |=B11110000; //PortD 4,5,6,7
  //servoPen.attach(3);
}
 
void loop(){
 for(int n=0;n<20;n++){
   //servoPen.write(100);
   for(int i=0;i<4;i++){
     PORTB &=0xf0;
     PORTB |= motorPen[i];
     PORTD &=0x0f;
     PORTD |= motorBall[i];
     delay(8);
   }
 }
 for(int n=0;n<20;n++){
   //servoPen.write(115);
   for(int i=3;i>=0;i--){
     PORTB &=0xf0;
     PORTB |= motorPen[i];
     PORTD &=0x0f;
     PORTD |= motorBall[i];
     delay(8);
   }
 }
}
