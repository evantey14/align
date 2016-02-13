
const int sensors = 5; 

int buzzerPins[] = {0,1,2,3,4}; // stores buzzer pins, add more for more buzzers
int calibrate[5]; // stores "straight-back" state, set based on testing, think of better name?
int sensorReadings[5]; // will store sensor input 0 to 1023

void setup() {
  Serial.begin(9600);
  // set output
  for(int i = 0; i<sensors; i++){
    digitalWrite(buzzerPins[i],OUTPUT);
  }
  // calibrate
  for(int i = 0; i<sensors; i++){
    calibrate[i] = 100; // change to some reading?
  }
}

void loop() {
  test();
  delay(1000);
}

void test(){
  int input = analogRead(A0);
  Serial.println(input);
}

void update(){
  //read new sensor values
  for(int i = 0; i<sensors; i++){
    sensorReadings[i] = analogRead(A0+i);
  }

  //test output w/ LEDS
  
  for(int i = 0; i<sensors; i++){
    if(sensorReadings[i]-calibrate[i]<0){
      digitalWrite(buzzerPins[i],HIGH);  
    } else {
      digitalWrite(buzzerPins[i],LOW);
    }
  }
}

