const int sensors = 4; // number of flex sensors/buzzers

//calibration
const int buttonPin = 0; // calibration button pin
unsigned long time_button = 0; // time the button was pressed
const long calibration_time = 3000; //calibration duration
const int num_cal_states = 1;
int cal_states[num_cal_states][sensors];
int cal_state_index = 0;  
int normal[sensors]; //stores "ideal posture"
  //for other "modes, add more rows, and a "mode" to select row
int buttonState = LOW; // HIGH when button pressed 

//reading
const int numReadings = 50;
int sensorReadings[sensors][numReadings]; // stores flex sensor input
int smoothReadings[sensors];
int readIndex = 0;


//error
int error[sensors]; // measure of incorrectness

//update time
//int threshold[] = {10,10,10,20}; // threshold error for buzz
int threshold[] = {4,8,5,10};
const long delay_time = 1500; // threshold "improper" time for buzz
unsigned long time_abnormal[sensors]; //length a sensors has been "improper"

//buzz
int buzzers[] = {6,3,5,4}; // buzzer pins (change to align with sensors)

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  for(int i = 0; i<sensors; i++){
    pinMode(buzzers[i],OUTPUT);
  }
  for(int i = 0; i<numReadings; i++){
    readSensors();
  }
  for(int i = 0; i<num_cal_states; i++){
    calibrate(LOW);
  }
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
    for(int i = 0; i<sensors; i++){
      normal[i] -= cal_states[cal_state_index][i]/num_cal_states;
    }
    while(millis()-time_button<calibration_time){
      readings++;
      readSensors();  
      for(int i = 0; i<sensors; i++){
        cal_states[cal_state_index][i] = cal_states[cal_state_index][i]+(smoothReadings[i]-cal_states[cal_state_index][i])/readings;
      }
    }
    for(int i = 0; i<sensors; i++){
      normal[i] += cal_states[cal_state_index][i]/num_cal_states;
    }
    cal_state_index=(cal_state_index+1)%num_cal_states;
  } 
}

void readSensors(){ // read sensor voltages into sensorReadings
  for(int i = 0; i<sensors; i++){ 
    //sensorReadings[i]=analogRead(A0+i);
    smoothReadings[i]=smoothReadings[i]-sensorReadings[i][readIndex]/numReadings;
    sensorReadings[i][readIndex]=analogRead(A0+i);
    smoothReadings[i]=smoothReadings[i]+sensorReadings[i][readIndex]/numReadings;
  }
  readIndex=(readIndex+1)%numReadings;
  //p(sensorReadings, (int)( sizeof(sensorReadings) / sizeof(sensorReadings[0])));
}

void findError(){ // quantify error between "ideal" and current posture
  for(int i = 0; i<sensors; i++){
    //error[i] = abs(smoothReadings[i]-normal[i]);
    error[i] = abs(smoothReadings[i]*500-normal[i]*500)/normal[i];
  }
  p(error, (int)( sizeof(error) / sizeof(error[0])));
}

void updateTime(){ // maintain amt of time in "bad posture" (if too long, buzz)
  for(int i = 0; i<sensors; i++){
    if(error[i]<threshold[i]){
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
 


