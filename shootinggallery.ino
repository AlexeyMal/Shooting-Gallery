// AMA 06.01.2021 
//
//For the circuit and application explanation see http://www.instructables.com/id/Bot-Laser-Gallery-Game/
//

#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn/DIN
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD/CS
 We have only a single MAX72XX.
 */
//LedControl lc=LedControl(12,11,10,1); //works. Any digital pins are allowed
// pin 2 is connected to the DataIn/DIN
// pin 4 is connected to the CLK 
// pin 3 is connected to LOAD/CS
LedControl lc=LedControl(2,4,3,1);
int score1, score2;

/*const byte numberOfBots = 5; 
const byte sensorPins[] = {2,4,6,8,10}; //{A0,A1,A2,A3,A4};    // All analog inputs
const byte ledPins[] = {3,5,7,9,11};      // feed back leds
*/
const byte numberOfBots = 6; 
const byte sensorPins[] = {5,7,9,11,A4,A2}; //{A0,A1,A2,A3,A4};    // All analog inputs
const byte ledPins[] = {6,8,10,12,A3,A1};      // feed back leds

//unsigned int sensorValues[numberOfBots];  // values of analogs
bool sensorValues[numberOfBots];  // values
bool botStates[numberOfBots];  //state of analogs //I realize now that I should have made a struct or class buttt..

//const byte resetButton = 8; //reset button pin 
//const byte speaker = 12; //10; //Speaker output
const byte speaker = A5; //12; //10; //Speaker output
//const byte upAdjust = 12;
//const byte downAdjust = 11;

//byte threshold =70;  //Might make this adjustable with button presses
bool isDone = false;
//byte ambient; //take a reading of the ambient light

//Checks for button presses and takes action
/*void handleButtons(){ 
  if (!digitalRead(resetButton)){
    resetBots();
    Serial.println("RESET All bots on!.........."); 
  }
  if (!digitalRead(upAdjust)){
    threshold+=10;
    buttonPress();
  }
  if (!digitalRead(downAdjust)){
    threshold-=10;
    buttonPress();
  }
}*/

/*
Print numbers on a 7-Segment display
The most common use of 7-segment displays is to print numbers. The LedControl library has a function that simply takes an argument of type byte and prints the corresponding digit on the specified column. Valid values for the digit are from 0 to 15 to allow displaying hex values . Values greater than 15 (or negative values) are silently discarded. The function also provides an argumment to switch decimal point on the column on or off.
Here is a code excerpt that prints an int value (-999..999) on a display with 4 digits. 
digit_offset - the position of the digit on the display (0..7) 
https://wayoda.github.io/LedControl/pages/software
*/
void printNumber(int v, int digit_offset) {  
    int ones;  
    int tens;  
    int hundreds; 

    boolean negative=false;

    if(v < -999 || v > 999)  
        return;  
    if(v<0) {  
        negative=true; 
        v=v*-1;  
    }
    ones=v%10;  
    v=v/10;  
    tens=v%10;  
    v=v/10; hundreds=v;  
    if(negative) {  
        //print character '-' in the leftmost column  
        lc.setChar(0,digit_offset+3,'-',false);  } 
    else {
        //print a blank in the sign column  
        lc.setChar(0,digit_offset+3,' ',false);  
    }  
    //Now print the number digit by digit 
    if(hundreds > 0) lc.setDigit(0,digit_offset+2,(byte)hundreds,false);
    else lc.setChar(0,digit_offset+2,' ',false);
    if((tens > 0)||(hundreds > 0)) lc.setDigit(0,digit_offset+1,(byte)tens,false); 
    else lc.setChar(0,digit_offset+1,' ',false);
    lc.setDigit(0,digit_offset+0,(byte)ones,false); 
} 


void handleSensors(){

  bool live1 = botStates[0] || botStates[1] || botStates[2];
  bool live2 = botStates[3] || botStates[4] || botStates[5];

  if(!live1) {
    botStates[(random() % 3)] = true; //[0 1 2] //set one random bot live
  }
  if(!live2) {
    botStates[(random() % 3 + 3)] = true; //[3 4 5] //set one random bot live
  }

  for (int i = 0; i < 3 ; i++){
    sensorValues[i] = digitalRead(sensorPins[i]); //Read each sensor, HIGH when the sensor is hit by laser
    if (sensorValues[i]) { 
        botStates[i] = false;
        handleNote();
        score1++;
    }
    digitalWrite(ledPins[i], botStates[i]); //Update all the bots
  }
  for (int i = 3; i < 6 ; i++){
    sensorValues[i] = digitalRead(sensorPins[i]); //Read each sensor, HIGH when the sensor is hit by laser
    if (sensorValues[i]) { 
        botStates[i] = false;
        handleNote2();
        score2++;
    }
    digitalWrite(ledPins[i], botStates[i]); //Update all the bots
  }
      
  //lc.clearDisplay(0);
  printNumber(score1, 4);
  printNumber(score2, 0);
  //delay(1500);
    
  if ((score1>=100)||(score2>=100)){ //GAME OVER
    //play something
    for (int i = 0; i < 10 ; i++){
      handleNote();
      lc.clearDisplay(0);
      handleNote2();
      printNumber(score1, 4);
      printNumber(score2, 0);
    }
    resetBots();
  } 
}

//It resets the ledStates to HIGH and takes another ambient light reading
void resetBots(){
   //ambient = 0;
   
  for (int i = 0; i < numberOfBots ; i++){
    //botStates[i] = true;
    //digitalWrite(ledPins[i], HIGH);
    //handleNote();
    botStates[i] = false;
    digitalWrite(ledPins[i], LOW);
    //ambient+=analogRead(sensorPins[i]); //Note this command can only happen after the ledPins are set to high
    }
  //ambient = ambient/numberOfBots; //Average of all the bots LDR readings
  
  score1 = 0;
  score2 = 0;
}

//Plays a sound
void handleNote(){
  unsigned int note = 4000;
  while (note > 1000){
    tone(speaker,note);
    delay(5);
    note-=100;
  }
  noTone(speaker);
}

//Plays a sound
void handleNote2(){
  unsigned int note = 1000;
  while (note < 4000){
    tone(speaker,note);
    delay(5);
    note+=100;
  }
  noTone(speaker);
}

void setup() {

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,4); //8 //brightness of the Leds can be set in 16 discrete steps(0..15). Higher values make the display brighter.
  /* and clear the display */
  lc.clearDisplay(0);
    
  pinMode(speaker, OUTPUT);
  
  // declare the ledPin as an OUTPUT:
  for (int i = 0; i < numberOfBots ; i++){
    pinMode(ledPins[i], OUTPUT);
  }

  //pinMode(resetButton, INPUT_PULLUP);
  //pinMode(upAdjust, INPUT_PULLUP);
  //pinMode(downAdjust, INPUT_PULLUP);
  
  //Serial.begin(9600);
  resetBots();//This will turn all the bots on
}

void loop() {
  //handleButtons();
  handleSensors();
}
