void setup() {
  Serial.begin(9600);
}

void loop() {
  char commands[20];
  commands[0] = 0;
  recvCmd(commands);
  
  Serial.print("command[0] : ");
  Serial.println(commands[0]);
  if(commands[0] == '$') {
    Serial.println("command recieved");
    
    int command_length = commands[1] + commands[2];
    Serial.print("command length : ");
    Serial.println(command_length);    
    /*
    if(commands[3] == 0x10) {
      Serial.println("MODE : setting turn character");
    }
    */
    
    switch(commands[3]) {  //コマンドにより分岐
      case 0x10:
        Serial.println("CMD : start turning");
      case 0x1F:
        Serial.println("CMD : stop turning");
      case 0x20:
        Serial.println("CMD : start capturing");
      case 0x2F:
        Serial.println("CMD : stop captureing");
      case 0x80:
        Serial.println("CMD : recieve turning symbol");
    }
        
    
  } else {
    Serial.println("unknown command");
  }
}


void recvCmd(char *buf) {
  int count = 0;
  char c;
  
  while(1) {
    if(Serial.available() > 0) {
      c = Serial.read();
      if(c == '\n') { 
        Serial.println("Line feed code was detected");
        break;
      } else {  
        buf[count] = c;
      }
      if(c == '%') break;
      count++;
    }
  }
  /*  -- for debug -- */
  Serial.print("recieved message : ");
  for(int i=0; i<=count; i++) {
    Serial.print(buf[i]);
    Serial.print(",");
  }
  
}
