#include <Stepper.h>

//PARAMETERS
#define STEPS 100  //モーターのステップ数
#define CLOSE_ARM_DEGREE 90
#define OPEN_ARM_DEGREE 2

Stepper stepper1(STEPS, 0, 1);
Stepper stepper2(STEPS, 2, 3);
Stepper stepper3(STEPS, 4, 5);
Stepper stepper4(STEPS, 6, 7);

char servo_code[4] = {servo1, servo2, servo3, servo4}
//サーボ
Servo servo1; 
Servo servo2;
Servo servo3;
Servo servo4;

int servo_pin[4] = {0, 1, 2, 3} //servo pin attach
int servo_status[4] = {0, 0, 0, 0} //0:open 1:close

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
void setup() {
  //起動前にステッピングモータをニュートラル位置に戻す
  //モータ・シリアルのセットアップ
    Serial.begin(9600);
    stepper.setSpeed(MOTOR_RPM); //モーターのスピード(rpm)    
    servo.attach(3);
  
  //準備完了したら、レディ状態を放出
    Serial.println("ready");
}

void stepper_control(int place, int move_step) {
    switch(place) {
        case 1:
            stepper1.step(move_step * STEPS);
        case 2:
            stepper2.step(move_step * STEPS);
        case 3:
            stepper3.step(move_step * STEPS);
        case 4:
            stepper4.step(move_step * STEPS):
    }
}
/* -USAGE-
    stepper_control(1, 100);    1番モータを100ステップ回転
*/

void cube_hold(int arm_number, bool hold_on) {
    if(hold_on == false) { //開ける        
        switch(place) {
            case 1:
                servo1.write(CLOSE_ARM_DEGREE);
            case 2:
                servo2.write(CLOSE_ARM_DEGREE);
            case 3:
                servo3.write(CLOSE_ARM_DEGREE);
            case 4:
                servo4.write(CLOSE_ARM_DEGREE);
        }  
    } 
    if(hold_on == true) { //閉める
        switch(place) {
            case 1:
                servo1.write(OPEN_ARM_DEGREE);
            case 2:
                servo2.write(OPEN_ARM_DEGREE);
            case 3:
                servo3.write(OPEN_ARM_DEGREE);
            case 4:
                servo4.write(OPEN_ARM_DEGREE);
        }
    }
}

/* -USAGE-
    cube_hold(1, true);     これで1番アームの腕を閉じる
*/

void turn_cube(int place, int ) {

}

void idle_cube(int place) {
	
}

void move_arm(int place, int arm_mode){


}


void rec_serial() {
  if(Serial.avaliable() > 0) {
    


void loop() {
  //キューブ画像認識　シリアルで画像認識開始コマンド到着したら、キューブの全面回転を開始
  
  
  //全面回転終わったら、次のコマンド到着まで待機
  //解法をシリアルで入手、シリアルが終われば随時解き始める
  
  //終了コマンド放出、アームの放出、ループ。
}

