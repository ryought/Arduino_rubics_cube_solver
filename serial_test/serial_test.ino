char turn_symbols[20] = {0};

void setup() {
  Serial.begin(9600);
}

//recvCmd : シリアルからのコマンド受信用
void recvCmd(char *buf) { 
  int count = 0; //受信文字数カウント用
  char c; //受信文字列の一時保管
  
  while(1) {
    if(Serial.available() > 0) {
      c = Serial.read();
      if(c == '\n') {  //改行コードは飛ばす
        Serial.println("Line feed code was detected");
        break;
      } else {  
        buf[count] = c;
      }
      if(c == '%') break;  //終端記号%でルーブ脱出
      count++;
    }
  }
  /*  -- for debug -- 受信メッセージ表示 */
  Serial.print("recieved message : ");
  for(int i=0; i<=count; i++) {
    Serial.print(buf[i]);
    Serial.print(",");
  }
  Serial.println("");
}


void move_arms(char *symbols, int com_len) {
  Serial.println(" : TURNING : ");
  for(int i=0; i<=3; i++) {
    Serial.println(symbols[i]);
  }
}




void loop() {
  char commands[50]; //コマンド用バッファ
  
  commands[0] = 0;
  recvCmd(commands); //受信コマンドをcommandsに
  
  Serial.print("command[0] : ");
  Serial.println(commands[0]);
  if(commands[0] == '$') { //先頭が$ならPCから送られたコマンド
    Serial.println("command recieved");
    
    int command_length = commands[1] + (commands[2] * 256); //コマンド長計算
    Serial.print("command length : ");
    Serial.println(command_length);

    switch(commands[3]) {  //コマンドにより分岐
      case 0x10:
        Serial.println("CMD : start turning");
        for(int inc=0; inc<20; inc++) {
          Serial.print("received : ");
          Serial.println(turn_symbols[inc]);   
        }
        Serial.println("CMD : exit");
        break;
      case 0x1F:
        Serial.println("CMD : stop turning");
        break;
      case 0x20:
        Serial.println("CMD : start capturing");
        break;
      case 0x2F:
        Serial.println("CMD : stop captureing");
        break;
      case 0x80:
        Serial.println("CMD : recieve turning symbol");
        //char turn_symbols[command_length-5];
        for(int inc=0; inc<=command_length-6; inc++) {
          turn_symbols[inc] = commands[inc+4];
          Serial.print("received : ");
          Serial.println(turn_symbols[inc]);   
          //commands配列から回転記号を抽出し、turn_symbols配列に入れる。
        }
        
        break;
    }    
    
  } else {
    Serial.println("unknown command");
  }

}



