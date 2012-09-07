int incommingByte;
int status_led = 8;
int com_size = 0;

void setup() {
  Serial.begin(9600);
  pinMode(status_led, OUTPUT);
  digitalWrite(status_led, LOW);
}


void loop() {
  if(Serial.available() > 3) {
    //digitalWrite(status_led, HIGH);
    incommingByte = Serial.read();
    
    Serial.println(incommingByte);
   
    if(incommingByte == '$') { //コマンドかどうか判
      digitalWrite(status_led, HIGH);
      if(Serial.available() > 0) {
        com_size = Serial.read();
        com_size += Serial.read() * 256;
        Serial.print("size : ");
        Serial.println(com_size);
      }
    } else {
      Serial.println("unknown command");
    }
  }
}
    
