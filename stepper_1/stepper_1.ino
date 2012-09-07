int pin[4] = {10,11,12,13};
 
void forward(int rot){
  for(int i=0;i<rot*120;i++){
    for(int i=0;i<4;i++){
       int prev = i-1;
       int next = i+1;
       if(prev < 0) prev = 3;
       if(next > 3) next = 0;
       digitalWrite(pin[prev], LOW);
       digitalWrite(pin[i], HIGH);
       digitalWrite(pin[next], HIGH);
       delay(2);
     }
   }
}
void reverse(int rot){
  for(int i=0;i<rot*120;i++){
     for(int i=3;i>=0;i--){
       int prev = i+1;
       int next = i-1;
       if(prev > 3) prev = 0;
       if(next < 0) next = 3;
       digitalWrite(pin[prev], LOW);
       digitalWrite(pin[i], HIGH);
       digitalWrite(pin[next], HIGH);
       delay(2);
     }
   }
}
 
void setup(){
  for(int i=0;i<4;i++){
    pinMode(pin[i], OUTPUT);
  }
}
 
void loop(){
  forward(100);
}
