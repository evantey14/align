const int sensors = 4; // number of flex sensors/buzzers

//calibration
const int buttonPin = 0; // calibration button pin
unsigned long time_button = 0; // time the button was pressed
const long calibration_time = 3000; //calibration duration
int normal[sensors]; //stores "ideal posture"
  //for other "modes, add more rows, and a "mode" to select row
int buttonState = LOW; // HIGH when button pressed 

//reading
int sensorReadings[sensors]; // stores flex sensor input

//error
int error[sensors]; // measure of incorrectness

//update time
const int threshold = 100; // threshold error for buzz
const long delay_time = 2000; // threshold "improper" time for buzz
unsigned long time_abnormal[sensors]; //length a sensors has been "improper"

//buzz
int buzzers[] = {3,4,5,6}; // buzzer pins (change to align with sensors)

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  for(int i = 0; i<sensors; i++){
    pinMode(buzzers[i],OUTPUT);
  }
  calibrate(LOW);
}

void loop() {
  calibrate(digitalRead(buttonPin));
  readSensors();
  findError();
  updateTime();
  delay(10);
}

void calibrate(int state){ // average state over [calibration_time] after button press
  buttonState = state;
  if(buttonState == LOW){
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

void readSensors(){ // read sensor voltages into sensorReadings
  for(int i = 0; i<sensors; i++){
    sensorReadings[i]=analogRead(A0+i);
  }
  //p(sensorReadings, (int)( sizeof(sensorReadings) / sizeof(sensorReadings[0])));
}

void findError(){ // quantify error between "ideal" and current posture
  for(int i = 0; i<sensors; i++){
    error[i] = sensorReadings[i]-normal[i];
  }
  p(error, (int)( sizeof(error) / sizeof(error[0])));
}

void updateTime(){ // maintain amt of time in "bad posture" (if too long, buzz)
  for(int i = 0; i<sensors; i++){
    if(error[i]<threshold){
      if(millis()-time_abnormal[i]>delay_time){buzz(i,0);}
      time_abnormal[i]=millis();
    } else if(millis()-time_abnormal[i]>delay_time){
      buzz(i,1);
    }
  }
}

void buzz(int i, int out){ // buzz digital output i based on out
    digitalWrite(buzzers[i],out);
}

void testA0(){ //test single raw sensor input at A0
  int input = analogRead(A0);
  Serial.println(input);
}

void p(int ray[], int size){ //print array
  for(int i = 0; i<size; i++){
    Serial.print(ray[i]);
    Serial.print(" ");
  }
  Serial.println();
}
 


