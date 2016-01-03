// 72U for TWO BIT CIRCUS, STEAM CARNIVAL
// Fall 2015
// Ver 0.1
// www.72u.org
// Dev: Nikita Rokotyan (http://rokotyan.com),  Luting Hou (http://lutinghou.com)
 
 import processing.serial.*;
 Serial myPort;        // The serial port
 int xPos = 1;         // horizontal position of the graph
 
 float rotationAngle, rotationAnglePrev;
 float soundEnvelope, soundEnvelopePrev;
 float soundThreshold, soundThresholdPrev;
 

 void setup () {
   size(700, 300);
   // if using Processing 2.1 or later, use Serial.printArray()
   println( Serial.list() );
   myPort = new Serial( this, Serial.list()[5], 9600 );   
   
   // don't generate a serialEvent() unless you get a newline character:
   myPort.bufferUntil('\n');
   background(0);

 }

 void serialEvent (Serial myPort) {
   String inString = myPort.readStringUntil('\n');
   //println("Data: " + inString);
   if ( inString != null && split( inString, ',' ).length == 3 ) {
     inString      = trim( inString );
     String[] strs = split( inString, ',' );
  
     getData( strs );
     drawData();
     
     if (xPos >= width) {
       xPos = 0;
       background(0);
     } else xPos++;
   }
 }
 
void draw () {} // everything happens in the serialEvent()

void getData( String[] strs ) {
     rotationAnglePrev  = rotationAngle;
     soundEnvelopePrev  = soundEnvelope;
     soundThresholdPrev = soundThreshold;
     
     rotationAngle  = float( strs[0] );
     soundEnvelope  = float( strs[1] );
     soundThreshold = float( strs[2] );
     
     rotationAngle  = map(rotationAngle, 5, 173, 0, height/2);
     soundEnvelope  = map(soundEnvelope, 0, 1024, 0, height/2);
     soundThreshold = map(soundThreshold, 0, 1024, 0, height/2);
     
}

void drawData() {
     strokeWeight(2);
     stroke(255,255,0); line(xPos-1, height-rotationAnglePrev, xPos, height - rotationAngle);
     stroke(255,34,45); line(xPos-1, height/2-soundEnvelopePrev, xPos, height/2 - soundEnvelope);
     
     strokeWeight(1);
     stroke(125,125,125);  line(xPos-1, height/2-soundThresholdPrev, xPos, height/2 - soundThreshold);
}
