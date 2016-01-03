// MIC2SERVO
// 72U for TWO BIT CIRCUS, STEAM CARNIVAL
// Fall 2015
// Ver 0.5
// www.72u.org
// Dev: Nikita Rokotyan (http://rokotyan.com), Luting Hou (http://lutinghou.com)

#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 
 
int rotationAngle = 0; 
const int   zeroAngle = 5;
const int   maxAngle = 170;

int      soundThreshold = 250;
int      soundEnvelope;
int      soundEnvelopeArr[50];
int      sIdx = 0;

float       duration = 0.0f; // 0.0 — 1.0
const float durationStep = 0.007f;

unsigned long timer = millis();


void setup() { 
  Serial.begin(9600);
  myservo.attach(8);  // attaches the servo on pin 9 to the servo object 
  for ( int i = 0; i < 50; ++i )
    soundEnvelopeArr[i] = 0;
} 
 
void loop() { 
  soundEnvelope  = analogRead(A1); 
  soundThreshold = analogRead(A2); 
  soundEnvelopeArr[sIdx] = soundEnvelope;
  sIdx < 50 ? sIdx++ : sIdx = 0;
  int soundEnveopeAvg = getAverage( soundEnvelopeArr, 50 );
  
  if ( soundEnveopeAvg > soundThreshold ) {
    duration += durationStep;
    timer = millis();
  } else if ( millis() - timer > 300 ) {
    duration -= 2*durationStep;
  }
  
  duration = constrain( duration, 0.0f, 1.0f );
  int rotationAngle = (int)(duration*maxAngle);
  myservo.write( rotationAngle );

  int serialData[] = { rotationAngle, soundEnveopeAvg, soundThreshold }; 
  sendDataToSerial( serialData, 3);
 
  delay(10);
} 


void sendDataToSerial( int arr[], int arrLength ) {
  String dataString = "";
  for ( int i = 0; i < arrLength-1; ++i ) {
    dataString += arr[i];
    dataString += ",";
  }
  dataString += arr[ arrLength-1 ];
  Serial.println( dataString );
}

int getAverage( int* arr, int n ) {
    int avg = 0;
    for ( int i = 0; i < n; ++i )
      avg += arr[i];
      
    avg /= n;
    return avg;
}
