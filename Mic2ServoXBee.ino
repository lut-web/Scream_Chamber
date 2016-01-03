// MIC2RELAY
// 72U for TWO BIT CIRCUS, STEAM CARNIVAL
// Fall 2015
// Ver 0.5
// www.72u.org
// Dev: Nikita Rokotyan (http://rokotyan.com), Luting Hou (http://lutinghou.com)


int      soundThreshold = 250;
int      soundEnvelope;

float       duration = 0.0f; // 0.0 — 1.0
const float durationStep = 0.0022f;
unsigned long timer = millis();

void setup() {
  Serial.begin( 9600 );
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop() {
  soundEnvelope  = analogRead(A1);
  soundThreshold = analogRead(A2);

  if ( soundEnvelope > soundThreshold ) {
    duration += 5*durationStep;
    timer = millis();
  } else if ( millis() - timer > 1000 ) {
    duration -= 4*durationStep;
  }
  duration = constrain( duration, 0.0f, 1.0f );
  
  if ( duration > 0 ) { 
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  }
  else {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }

  int serialData[] = { (int)(duration*100), soundEnvelope, soundThreshold }; 
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
