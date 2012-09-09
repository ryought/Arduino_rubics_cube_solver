#define STEPS 72  //for kopal 1702
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int servo_pin[4]={4,5,6,7};
int servo_status[4]={};
int motor_pin[4][4]={{16,17,18,19},{8,9,10,11},{12,13,14,15},{0,1,2,3}};
//このへんは要調整。0,1は使えないみたい。


int s_deg; //servo_deg

/*
ARM_LAYOUT
    [3]
[2]     [4]
    [1]

TURN_COMMAND
 1 : turn 90 degree (Clockwise rotation)
 2 : turn 180 degree (Clockwise rotation)
 -1: turn -90 degree (Counterclockwise rotation)
 -2: turn -180 degree (Counterclockwise rotation)

COMMAND_LISTI
 - stepper_write(
 - stepper_control(ARM_NUMBER 腕番号, MOVE_STEP 回転量(1,2,-1,-2等))
    指定番号の腕を回転量だけ回転させる。
 - cube_hold(ARM_NUMBER 腕番号, TRUE(HOLD)/FALSE(RELEASE))
    指定された腕番号の腕で、キューブを掴ませるか否か

 - turn_cube(ARM_NUMBER 腕番号, ROTATION_AMOUNTS 回転量(1,2,-1,-2等))
    指定された腕番号の腕で、キューブを回転量分だけ回転させる　1ブロックだけ回転
    cube_holdが追加された感じ
 - idle_cube(ARM_NUMBER 腕番号, ROTATION_AMOUNTS 回転量)
    空回りさせる。向き回転用。
 
 - solve_cube(SURFACE_CODE 面のコード, ROTATION_AMOUNTS 回転量)
    回転記号を渡せばそのとおりに動くようなもの。変換はまあ、やってくださいよ
 -
*/

/*考えるメモ

最小移動単位は90度。→90度回すということは、一回転ステップ数/4だけステップさせる。
モーターは4つある。それぞれのピンについてやる。→2次元配列。

*/

void forward(int motor_num, int rot){  //1rot = 90 deg
  for(int l=0;l<rot*STEPS/4;l++){
    /*dbg
    Serial.print(l);
    Serial.print(" : ");
    */
    for(int i=0;i<4;i++){
       //Serial.print(i);
       int prev = i-1;
       int next = i+1;
       if(prev < 0) prev = 3;
       if(next > 3) next = 0;
       digitalWrite(motor_pin[motor_num-1][prev], LOW);
       digitalWrite(motor_pin[motor_num-1][i], HIGH);
       digitalWrite(motor_pin[motor_num-1][next], HIGH);
       delay(8);
     }
   //Serial.println("");
   }
   delay(1000);
}

void reverse(int motor_num, int rot){
  for(int l=0;l<rot*STEPS/4;l++){
     for(int i=3;i>=0;i--){
       int prev = i+1;
       int next = i-1;
       if(prev > 3) prev = 0;
       if(next < 0) next = 3;
       digitalWrite(motor_pin[motor_num-1][prev], LOW);
       digitalWrite(motor_pin[motor_num-1][i], HIGH);
       digitalWrite(motor_pin[motor_num-1][next], HIGH);
       Serial.println(motor_pin[motor_num-1][0]);
       delay(8);
     }
   }
   delay(1000);
}

void setup() {
  //serial(pc)
  Serial.begin(9600);
  
  //servo
  servo1.attach(servo_pin[0]);
  servo2.attach(servo_pin[1]);
  servo3.attach(servo_pin[2]);
  servo4.attach(servo_pin[3]);
  
  //stepper
  for(int i=4; i<=20; i++){
    pinMode(i, OUTPUT);
  }  
}

void loop() {
  s_deg = 28;
  Serial.print("Servo : ");
  Serial.println(s_deg);
  servo1.write(s_deg);
  Serial.println(servo_pin[0]);
  Serial.println("forward 1");
  forward(1, 1); // 1回正転
  
  s_deg = 110;
  Serial.print("Servo : ");
  Serial.println(s_deg);
  servo1.write(s_deg);
  
  Serial.println("reverse 1");
  reverse(1, 1); // 1回逆転
}
