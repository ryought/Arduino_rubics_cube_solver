char turn_symbols;

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


//define_symbol : 解法受信と、使える形にして別の配列に移動。
//define_symbol(受信したコマンドの配列, 結果出力用の配列)
char define_symbol(char *buf, int com_len) {
  char turn_symbols[com_len - 5];
  for(int i=0; i <= com_len-6; i++) {
    turn_symbols[i] = buf[i+4];
    Serial.print("turn_symbols : ");
    Serial.print(i+4);
    Serial.print(" ");
    Serial.println(buf[i+4]);
  }
  return *turn_symbols;
}


void move_arms(char *symbols, int com_len) {
  Serial.print("-TURNING ");
  for(int i=0; i<=com_len; i++) {
    Serial.print(symbols[i]);
  }
  Serial.println("-");
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
        move_arms(turn_symbols, command_length);
      case 0x1F:
        Serial.println("CMD : stop turning");
      case 0x20:
        Serial.println("CMD : start capturing");
      case 0x2F:
        Serial.println("CMD : stop captureing");
      case 0x80:
        Serial.println("CMD : recieve turning symbol");
        turn_symbols = define_symbol(commands, command_length);
    }
        
    
  } else {
    Serial.println("unknown command");
  }
}



