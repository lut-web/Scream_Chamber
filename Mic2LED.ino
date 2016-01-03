// MIC2LED, LED strip controlled by the duration of the sound.
// 72U for TWO BIT CIRCUS, STEAM CARNIVAL
// Fall 2015
// Ver 0.5
// www.72u.org
// Dev: Luting Hou (http://lutinghou.com), Nikita Rokotyan (http://rokotyan.com)

#include <Adafruit_DotStar.h>
#include <SPI.h>

// LED Vars
Adafruit_DotStar *LEDStrips;
const int numberOfStrips = 3;
const int dataPins[]  = {12, 6, 8};
const int clockPins[] = {13, 7, 9};
const int numPixels[] = {60, 35, 75};
float     headPrev = 0.0f;
float     head  = 0.0f; // Index of first 'on' and 'off' pixels
float     headStep = 0.015f;
uint32_t  color = 0xFFFF00;      // 'On' color (starts red)
float value = 0.0f; //value of the waiting pattern
boolean waitingUp = false; //waiting value is increasing
// Mic Vars
int       stopCounter = 0;
int       soundEnvelope;
int       soundThreshold = 50;

void setup() {
  Serial.begin(9600);
  LEDStrips = (Adafruit_DotStar *)malloc( 5*sizeof(Adafruit_DotStar) );
  for ( int i = 0; i < numberOfStrips; ++i ) {
    LEDStrips[i] = Adafruit_DotStar( numPixels[i], dataPins[i], clockPins[i], DOTSTAR_RGB );
    LEDStrips[i].begin();
    LEDStrips[i].show();
  }
}

void loop() {
  soundEnvelope = analogRead(A1);
  soundThreshold = analogRead(A2);
  
  // Calculate how long the scream has stopped
  if ( soundEnvelope <= soundThreshold) stopCounter++; 
  else stopCounter = 0; 
  
  // Pattern when no scream is detected within 300 * 5 millis
  if ( stopCounter > 300 ) {
    waitingLEDs();
  }
  // If LED is not fully lit, and scream is detected
  if ( soundEnvelope > soundThreshold) {        
    if ( head < 1.0f ) head += headStep;
    updateLEDs( false /*to the top*/ );
  } 
  // If scream stopped no longer than 50 * 5 millis, keep the LED
  else if (stopCounter < 50) { 
    updateLEDs( false /*to the top*/ );
  } 
  // If LED is partially or fully lit, and scream stopped between 50 * 5 and 300 * 5 millis
  else if (stopCounter > 50 && stopCounter < 300){ 
    if (head > 0) head -= headStep;
    updateLEDs( true /*backwards*/ );
  }

  headPrev = head;
  
  int serialData[] = { soundEnvelope, (int)(100*head), soundThreshold }; 
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


void updateLEDs( bool backwards) { // true - Forward, false - Backward
  float loopStart  = backwards ? head    : headPrev;
  float loopEnd    = backwards ? headPrev : head;
  for (float j = loopStart ; j < loopEnd; j += headStep )  {   
    for ( int k = 0; k < numberOfStrips; ++k ) {
      double hue = 0.15;
      int headLED = (int)(j*numPixels[k]);
      for (int i = 0; i < numPixels[k]; ++i ) { // Loop over all strips
        if ( hue < 0.72 ) hue += 0.57/numPixels[k]; 
        color = hsvToRgbColor( hue, 1.0, 1.0 );
        
        i < headLED ? LEDStrips[k].setPixelColor( i, color ) : LEDStrips[k].setPixelColor( i, 0 );
      }
      LEDStrips[k].show();
    } 
  }
  
}

void showStrips() {
  for ( int i = 0; i < numberOfStrips; ++i )
    LEDStrips[i].show();
}

void waitingLEDs() {
    if (value >= 0.4) waitingUp = false;
    if (value <= 0.0) waitingUp = true;
    if (waitingUp == true) value +=0.001;
    else  value -=0.001;
    for ( int k = 0; k < numberOfStrips; ++k ) {
      int headLED = (int)numPixels[k];
      for (int i = 0; i < numPixels[k]; ++i ) { // Loop over all strips
        color = hsvToRgbColor( 0.15, 1.0, value);       
        LEDStrips[k].setPixelColor( i, color );
      }
      LEDStrips[k].show();
    } 
}

uint32_t hsvToRgbColor(double h, double s, double v) {
    double r, g, b;

    int i = (int)(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);
    switch(i % 6){
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }
    uint32_t color = ( ((uint32_t)(255*r) & 0xff ) << 16 ) + ( ((uint32_t)(255*g) & 0xff) << 8 ) + ( (uint32_t)(255*b) & 0xff );
    return color;
}

