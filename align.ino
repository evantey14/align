
const int sensors = 5;
const int numReadings = 50;

int buzzerPins[] = {0,1,2,3,4}; // stores buzzer pins, add more for more buzzers
int initial[sensors]; // stores "straight-back" state, set based on testing, think of better name?
int rawRead[sensors][numReadings]; // will store last 100 sensor input 0 to 1023
int aggRead[sensors];
int smoothRead[sensors];
int readIndex = 0;

void setup() {
  Serial.begin(9600);
  // set output
  for(int i = 0; i<sensors; i++){
    digitalWrite(buzzerPins[i],OUTPUT);
  }
  //calibrate();
}

void loop() {
  readSensors();
  delay(10);
}

void calibrate(){
  delay(1);
}

void testA0(){
  int input = analogRead(A0);
  Serial.println(input);
}

void readSensors(){
  for(int i = 0; i<sensors; i++){
    smoothRead[i]=analogRead(A0+i);
    Serial.print(smoothRead[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void smoothReadSensors(){
  //read new sensor values
  for(int i = 0; i<sensors; i++){
    aggRead[i]-=rawRead[i][readIndex];
    rawRead[i][readIndex] = analogRead(A0+i);
    aggRead[i]+=rawRead[i][readIndex];
    smoothRead[i]=aggRead[i]/numReadings;
    Serial.print(smoothRead[i]);
    Serial.print(" ");
  }
  readIndex = (readIndex+1) % numReadings;
  Serial.println();
}
 


