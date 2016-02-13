
const int sensors = 4;
const int numReadings = 50;
const int buttonPin = 2;
const long calibration_time = 3000;
int buzzerPins[] = {13,1,2,3,4}; // stores buzzer pins, add more for more buzzers
int normal[sensors]; 
int sensorReadings[sensors];
int error[sensors];
int readIndex = 0;
unsigned long time_button = 0;
unsigned long time_abnormal = 0;
int buttonState = 0;


void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  for(int i = 0; i<sensors; i++){
    pinMode(buzzerPins[i],OUTPUT);
  }
  buttonState = HIGH;
}

void loop() {
  
  calibrate();
  readSensors();
  findError();
  //updateTime();
  //buzz
  buttonState = LOW;
  delay(10);
}

void calibrate(){
  //buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
    time_button = millis();
    int readings = 0;
    while(millis()-time_button<calibration_time){
      readings++;
      readSensors();
      for(int i = 0; i<sensors; i++){
        normal[i] = normal[i]+(sensorReadings[i]-normal[i])/readings;
      }
    }
  } 
}

void readSensors(){
  for(int i = 0; i<sensors; i++){
    sensorReadings[i]=analogRead(A0+i);
  }
  //p(sensorReadings, (int)( sizeof(sensorReadings) / sizeof(sensorReadings[0])));
}

void findError(){
  for(int i = 0; i<sensors; i++){
    error[i] = sensorReadings[i]-normal[i];
  }
  p(error, (int)( sizeof(error) / sizeof(error[0])));
}

void testA0(){
  int input = analogRead(A0);
  Serial.println(input);
}

void p(int ray[], int size){
  for(int i = 0; i<size; i++){
    Serial.print(ray[i]);
    Serial.print(" ");
  }
  Serial.println();
}
 


