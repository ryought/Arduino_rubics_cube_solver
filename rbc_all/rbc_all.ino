#define STEPS 72  //for kopal 1702
#define CLOSE_DEG 20
#define OPEN_DEG 101
#define MOTOR_SPEED 20
#define TURN_DELAY 1000
#include <Servo.h>

Servo servo1; //1と3
Servo servo2; //2と4

int servo_pin[2]={2,3};
int servo_status[4]={};
int motor_pin[4][4]={{4,5,6,7},{8,9,10,11},{12,13,14,15},{16,17,18,19}};
//このへんは要調整。0,1は使えないみたい。

char turn_symbols[60] = {0};

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

void setup() {
  //serial(pc)
  Serial.begin(9600);
  

  //stepper
  for(int i=0; i<=20; i++){
    pinMode(i, OUTPUT);
  }

  //servo
  
  servo1.attach(servo_pin[0]);
  servo2.attach(servo_pin[1]);
}



void forward(int motor_num, int rot){  //1rot = 90 deg
  for(int l=0;l<rot*STEPS/4;l++){
    for(int i=0;i<4;i++){
       int prev = i-1;
       int next = i+1;
       if(prev < 0) prev = 3;
       if(next > 3) next = 0;
       digitalWrite(motor_pin[motor_num-1][prev], LOW);
       digitalWrite(motor_pin[motor_num-1][i], HIGH);
       digitalWrite(motor_pin[motor_num-1][next], HIGH);
       delay(MOTOR_SPEED);
     }
   //Serial.println("");
   }
   delay(TURN_DELAY);
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
       delay(MOTOR_SPEED);
     }
   }
   delay(TURN_DELAY);
}



//2つのモータの同時回転用。空回り用
void dual_forward(int motor_num_1, int motor_num_2, int rot) {
  int i_2;
  for(int l=0;l<rot*STEPS/4;l++){
   
    for(int i=0;i<4;i++){
       switch(i) {
         case 0:
           i_2 = 0;
           break;
         case 1:
           i_2 = 3;
           break;
         case 2:
           i_2 = 2;
           break;
         case 3:
           i_2 = 1;
           break;
       }
       //Serial.print(i);
       int prev_1 = i-1;
       int next_1 = i+1;
       int prev_2 = i_2 + 1;
       int next_2 = i_2 - 1;
       if(prev_1 < 0) prev_1 = 3;
       if(prev_2 > 3) prev_2 = 0;
       if(next_1 > 3) next_1 = 0;
       if(next_2 < 0) next_2 = 3;
       
       digitalWrite(motor_pin[motor_num_1 - 1][prev_1], LOW);
       digitalWrite(motor_pin[motor_num_2 - 1][prev_2], LOW);
       digitalWrite(motor_pin[motor_num_1 - 1][i], HIGH);
       digitalWrite(motor_pin[motor_num_2 - 1][i_2], HIGH);
       digitalWrite(motor_pin[motor_num_1 - 1][next_1], HIGH);
       digitalWrite(motor_pin[motor_num_2 - 1][next_2], HIGH);
       delay(MOTOR_SPEED);
     }
   //Serial.println("");
   }
   delay(TURN_DELAY);
}



//2つのモータの同時回転用。空回り用
void dual_reverse(int motor_num_1, int motor_num_2, int rot) {
  int i_2;
  for(int l=0;l<rot*STEPS/4;l++){
   
    for(int i=3;i>=0;i--){
       switch(i) {
         case 0:
           i_2 = 0;
           break;
         case 1:
           i_2 = 3;
           break;
         case 2:
           i_2 = 2;
           break;
         case 3:
           i_2 = 1;
           break;
       }
       //Serial.print(i);
       int prev_1 = i+1;
       int next_1 = i-1;
       int prev_2 = i_2 - 1;
       int next_2 = i_2 + 1;
       if(prev_1 > 3) prev_1 = 0;
       if(prev_2 < 0) prev_2 = 3;
       if(next_1 < 0) next_1 = 3;
       if(next_2 > 3) next_2 = 0;
       
       digitalWrite(motor_pin[motor_num_1 - 1][prev_1], LOW);
       digitalWrite(motor_pin[motor_num_2 - 1][prev_2], LOW);
       digitalWrite(motor_pin[motor_num_1 - 1][i], HIGH);
       digitalWrite(motor_pin[motor_num_2 - 1][i_2], HIGH);
       digitalWrite(motor_pin[motor_num_1 - 1][next_1], HIGH);
       digitalWrite(motor_pin[motor_num_2 - 1][next_2], HIGH);
       delay(MOTOR_SPEED);
     }
   //Serial.println("");
   }
   delay(TURN_DELAY);
}

void stepper_control(int motor_num, int rot) { //完成
  if(rot < 0) {
    reverse(motor_num, -rot);
  }
  if(rot > 0) {
    forward(motor_num, rot);
  }
  /*dbg
  Serial.print("MOVING motor_num : ");
  Serial.print(motor_num);
  Serial.print(" rot : ");
  Serial.print(rot);
  Serial.println(" ");
  */
}

void cube_hold(int motor_num, boolean lock_status) { //cube_hold(1, true)で1番を閉じる 完成
  if(lock_status == true) {
    switch(motor_num) {
      case 1:
        servo1.write(CLOSE_DEG);
        break;
      case 2:
        servo2.write(CLOSE_DEG);
        break;
      case 3:
        servo1.write(CLOSE_DEG);
        break;
      case 4:
        servo2.write(CLOSE_DEG);
        break;
    }
  }
  if(lock_status == false) {
    switch(motor_num) {
      case 1: //1と3
        servo1.write(OPEN_DEG);
        break;
      case 2: //2と4
        servo2.write(OPEN_DEG);
        break;
      case 3:
        servo1.write(OPEN_DEG);
        break;
      case 4:
        servo2.write(OPEN_DEG);
        break;
    }
  }
  delay(1000);
}




void idle_cube(int motor_num, int rot) {
  switch(motor_num){
    case 1: //1と3でつかんで、空回りさせる
      cube_hold(1, true);
      cube_hold(3, true);
      cube_hold(2, false);
      cube_hold(4, false);
      //回す 1をrot, 3を-rotだけ回転させる
      stepper_control(1, rot);
      stepper_control(3, -rot);
      //腕の位置復帰
      //2,4でつかんで、1,3をはなし、位置を戻す
      cube_hold(1, false);
      cube_hold(3, false);
      cube_hold(2, true);
      cube_hold(4, true);
      //元の位置まで戻す回転
      stepper_control(1, -rot);
      stepper_control(3, rot);
      break;
      
    case 2: //2と4でつかんで、空回りさせる
      cube_hold(2, true);
      cube_hold(4, true);
      cube_hold(1, false);
      cube_hold(3, false);
      //回す 2をrot, 4を-rotだけ回転させる
      stepper_control(2, rot);
      stepper_control(4, -rot);
      //腕の位置復帰
      //1,3でつかんで、2,4をはなし、位置を戻す
      cube_hold(2, false);
      cube_hold(4, false);
      cube_hold(1, true);
      cube_hold(3, true);
      //元の位置まで戻す回転
      stepper_control(2, -rot);
      stepper_control(4, rot);
      break;
  }
}


//recvCmd : シリアルからのコマンド受信用
void recvCmd(char *buf) { 
  int count = 0; //受信文字数カウント用
  char c; //受信文字列の一時保管
  
  while(1) {
    if(Serial.available() > 0) {
      c = Serial.read();
      if(c == '\n') {  //改行コードは飛ばす and 終端文字としての\n
        /*
        Serial.println("Line feed code was detected");
        */
        break;
      } else {  
        buf[count] = c;
      }
      /*
      if(c == '%') break;  //終端記号%でルーブ脱出
      */
      count++;
    }
  }
  /*  -- for debug -- 受信メッセージ表示 
  Serial.print("message received");
  /*
  Serial.print("recieved message : ");
  for(int i=0; i<=count; i++) {
    Serial.print(buf[i]);
    Serial.print(",");
  }
  Serial.println("");
  */
}

void reply(int comm) {
  Serial.print('#');
  Serial.write(0x05);
  Serial.write((uint8_t)0x00);
  Serial.write(comm);
  Serial.print('\n'); 
}

void define_symbols() {
  String def_symbols = turn_symbols;
  /*
  Serial.println(def_symbols.charAt(0));
  */

  for(int i=0; i<=60; i++) {
    /*
    Serial.print("seeking at : ");
    Serial.println(i);
    */
    switch(def_symbols.charAt(i)) {
      case 'F': //1
        //Serial.println("found : F");
        //回す処理＋ホールド処理
        cube_hold(1, true);
        cube_hold(2, true);
        
        if(def_symbols.charAt(i+1) == '2') {
          stepper_control(1, 2);
          cube_hold(1, false);
          stepper_control(1, -2);
          cube_hold(1, true);
        }
        if(def_symbols.charAt(i+1) == '\'') {
          stepper_control(1, -1);
          cube_hold(1, false);
          stepper_control(1, 1);
          cube_hold(1, true);
        }
        if(def_symbols.charAt(i+1) == ' ') {
          stepper_control(1, 1);
          cube_hold(1, false);
          stepper_control(1, -1);
          cube_hold(1, true);
        }
        break;
      case 'B': //3
        //Serial.println("found : B");
        //回す処理＋ホールド処理
        cube_hold(1, true);
        cube_hold(2, true);
        
        if(def_symbols.charAt(i+1) == '2') {
          stepper_control(3, 2);
          cube_hold(3, false);
          stepper_control(3, -2);
          cube_hold(3, true);
        }
        if(def_symbols.charAt(i+1) == '\'') {
          stepper_control(3, -1);
          cube_hold(3, false);
          stepper_control(3, 1);
          cube_hold(3, true);
        }
        if(def_symbols.charAt(i+1) == ' ') {
          stepper_control(3, 1);
          cube_hold(3, false);
          stepper_control(3, -1);
          cube_hold(3, true);
        }
        break;
      case 'R'://4
        //Serial.println("found ; R");
        //回す処理＋ホールド処理
        cube_hold(1, true);
        cube_hold(2, true);
        
        if(def_symbols.charAt(i+1) == '2') {
          stepper_control(4, 2);
          cube_hold(4, false);
          stepper_control(4, -2);
          cube_hold(4, true);
        }
        if(def_symbols.charAt(i+1) == '\'') {
          stepper_control(4, -1);
          cube_hold(4, false);
          stepper_control(4, 1);
          cube_hold(4, true);
        }
        if(def_symbols.charAt(i+1) == ' ') {
          stepper_control(4, 1);
          cube_hold(4, false);
          stepper_control(4, -1);
          cube_hold(4, true);
        }
        break;
      case 'L'://2
        //Serial.println("found : L");
        //回す処理＋ホールド処理
        cube_hold(1, true);
        cube_hold(2, true);
        
        if(def_symbols.charAt(i+1) == '2') {
          stepper_control(2, 2);
          cube_hold(2, false);
          stepper_control(2, -2);
          cube_hold(2, true);
        }
        if(def_symbols.charAt(i+1) == '\'') {
          stepper_control(2, -1);
          cube_hold(2, false);
          stepper_control(2, 1);
          cube_hold(2, true);
        }
        if(def_symbols.charAt(i+1) == ' ') {
          stepper_control(2, 1);
          cube_hold(2, false);
          stepper_control(2, -1);
          cube_hold(2, true);
        }
        break;
      
      case '\'' | '2' | ' ' | ')':
        //Serial.println("found : ', 2,  , )");
        //逆向きに回す処理　　次の文字を読んで判定
        continue;
        
      case '(':
        //Serial.println("found : (");
        //一周回転用
        switch(def_symbols.charAt(i+1)) {
          case 'f': //1, 3を回転させる
            if(def_symbols.charAt(i+2) == '\'') {
              //一回逆からまわり
              idle_cube(1, -1);
            }
            if(def_symbols.charAt(i+2) == ')') {
              //一回正からまわり
              idle_cube(1, 1);
            }
            break;
          case 'r': //2, 4を回転させる
            if(def_symbols.charAt(i+2) == '\'') {
              //一回逆からまわり
              idle_cube(2, -1);
            }
            if(def_symbols.charAt(i+2) == ')') {
              //一回正からまわり
              idle_cube(2, 1);
            }
            break;
        }
    }
  }
}
    

void loop() {
    stepper_control(1, 4);
  Serial.print("1");
  stepper_control(2, 4);
  Serial.print("2");
  
  stepper_control(3, 4);
  Serial.print("3");
  stepper_control(4, 4);
  Serial.print("4");
  
  char commands[50]; //コマンド用バッファ
  
  commands[0] = 0;
  recvCmd(commands); //受信コマンドをcommandsに
  
  //Serial.print("command[0] : ");
  //Serial.println(commands[0]); 
  if(commands[0] == '$') { //先頭が$ならPCから送られたコマンド
    //Serial.println("command recieved");
    
    int command_length = commands[1] + (commands[2] * 256); //コマンド長計算
    //Serial.print("command length : ");
    //Serial.println(command_length);

    switch(commands[3]) {  //コマンドにより分岐
    /*送られてくる可能性のあるコマンド一覧
      0x10 16キャプチャ開始→reply(0x90), キャプチャの時の手順で回転させる(0x70)
      0x1F 31キャプチャ終了→reply(0x9F)
      0x20 32 ソルブ開始→reply(0xA0)→回転記号の到着まで待つ
      0x70 112 回転記号送信→reply(0xF0)→define_symbolsで回転実行
      
      送信するコマンド
      0xFF 回転終了
    
    
    */
      case 0x10: //CMD: start capture
        /*
        Serial.println("CMD : start capturing");
        */
        //reply
        reply(0x90);
        break;
        
      case 0x1F: //CMD: end capture
        /*
        Serial.println("CMD : stop turning");
        */
        //reply
        reply(0x9F);
        break;
        
      case 0x20: //CMD: ソルブ開始
        reply(0xA0);
        break;
      case 0x70: //CMD: 回転記号受け取りと、回転
        //Serial.println("CMD : recieve turning symbol");
        //char turn_symbols[command_length-5];
        for(int inc=0; inc<=command_length-6; inc++) {
          turn_symbols[inc] = commands[inc+4];
          /*
          Serial.print("received : ");
          Serial.println(turn_symbols[inc]);   
          */
          //commands配列から回転記号を抽出し、turn_symbols配列に入れる。
        }
        //そのまま回転開始 
        reply(0xF0); //回転記号OK
        define_symbols();
        reply(0xFF); //回転終了
        break;
    }    
    
  } else {
    Serial.println("unknown command");
  }
  
  /*
  cube_hold(1, true);
  stepper_control(1, 4);
  stepper_control(1, -1);
  cube_hold(1,false);
  delay(1000);
  */
  /*
  cube_hold(1, true);
  cube_hold(2, true);
  cube_hold(3, true);
  cube_hold(4, true);
  */
}
