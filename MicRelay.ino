// MIC2RELAY
// 72U for TWO BIT CIRCUS, STEAM CARNIVAL
// Fall 2015
// Ver 0.5
// www.72u.org
// Dev: Nikita Rokotyan (http://rokotyan.com), Luting Hou (http://lutinghou.com)


int      soundThreshold = 250;
int      soundEnvelope;
int      soundEnvelopeArr[50];
int      sIdx = 0;

float       duration = 0.0f; // 0.0 — 1.0
const float durationStep = 0.0022f;
unsigned long timer = millis();

void setup() {
  Serial.begin( 9600 );
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);

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
    duration += 5*durationStep;
    timer = millis();
  } else if ( millis() - timer > 1000 ) {
    duration -= 2*durationStep;
  }
  duration = constrain( duration, 0.0f, 1.0f );
  
  if ( duration > 0 ) { 
    digitalWrite(A4, LOW);
    digitalWrite(A5, LOW);
  }
  else {
    digitalWrite(A4, HIGH);
    digitalWrite(A5, HIGH);
  }

  int serialData[] = { (int)(duration*100), soundEnveopeAvg, soundThreshold }; 
  sendDataToSerial( serialData, 3);
  
  delay(5);
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
