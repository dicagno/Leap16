/*
 *
 *
 * Leap16 firmware
 * 8-bit digital 16 step MIDI sequencer and controller
 *
 * Copyright (c) 2013 albertodicagno.github.io
 *
 */

#include "digitalIOPerformance.h"

#include <analogmuxdemux.h>

#include <Bounce.h>

#include <EEPROM.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(53, 52, 50, 46, 48);

#define LCD_VCC 47
#define LCD_GND 49
//#define LCD_BL 51 //-> Connected to 3.3v Arduino's onboard regulator

#define READPIN A13

AnalogMux amux(39,37,35, READPIN);

int clockSourceSelectToggle = 18;

int pots[] = {4, 5, 7, 3, 2, 6, 0, 1, 15, 14, 10, 9, 12, 8, 11, 13};
int leds[] = {4, 5, 6, 7, 8, 9, 10, 11, 24, 26, 28, 30, 32, 34, 36, 38};
int duration, lastValue, stepID = 0;
int maxPeriod = 1000;
int minPeriod = 20;
int playbackState = true;
int midiClock = digitalRead(clockSourceSelectToggle);
int statusPin = 13;
int PWMPin = 12;
int btnDown = 20;
int btnUp = 21;
int btnEnter = 19;

int quantizationFactor = 24; // 1/4 note
int _quantizationF = 96/quantizationFactor;
int beatCounter = 0;
int beatID = 0;
int last_led = 0;

int totalSteps = 16;

int editoctaveRange = false;
int midiDisplayInit = false;

boolean buttonPressedUp;
boolean buttonPressedDown;
boolean buttonPressedEnter;

int tickID = true;
int midiLastValue = 0;

Bounce bouncerUp = Bounce(btnUp, 5); 
Bounce bouncerDown = Bounce(btnDown, 5); 
Bounce bouncerEnter = Bounce(btnEnter, 5); 
Bounce bouncerStartStop = Bounce(3, 5); 

byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;
int play_flag = false;
byte data;
int counter = 0;
int menuLevel = 0;
int selectedMenu = 0;

int editTrspMax = false;

int octaveRange = EEPROM.read(120);
int trspMax = EEPROM.read(121);

int absoctaveRange = 0;
int absTrspMax = 127;
int _beatCount = 0;

//New
int clock_step;
int note;
int noteval;
int LowestNote;
int HighestNote;

int Note1;
int Note2;
int Note3;
int Note4;
int Note5;
int Note6;
int Note7;
int Note8;
int Note9;
int Note10;
int Note11;
int Note12;
int Note13;
int Note14;
int Note15;
int Note16;

int SeqLength;

void sendNoteOn (byte note_, byte vel_, byte ch_){
Serial.write(144);
Serial.write(note_);
Serial.write(vel_);
}

void sendNoteOff (byte note_, byte vel_, byte ch_){
Serial.write(128);
Serial.write(note_);
Serial.write(vel_);
}

void clkInit() {
  clock_step=0;
note = 0x3F;
noteval = 0;
LowestNote=36;
HighestNote=36+octaveRange*12; //3 Octaves over LowestNote


Note1 = (96*1)/_quantizationF;
Note2 = (96*2)/_quantizationF;
Note3 = (96*3)/_quantizationF;
Note4 = (96*4)/_quantizationF;
Note5 = (96*5)/_quantizationF;
Note6 = (96*6)/_quantizationF;
Note7 = (96*7)/_quantizationF;
Note8 = (96*8)/_quantizationF;
Note9 = (96*9)/_quantizationF;
Note10 = (96*10)/_quantizationF;
Note11 = (96*11)/_quantizationF;
Note12 = (96*12)/_quantizationF;
Note13 = (96*13)/_quantizationF;
Note14 = (96*14)/_quantizationF;
Note15 = (96*15)/_quantizationF;
Note16 = (96*16)/_quantizationF;

SeqLength = quantizationFactor*16; //Sequence length is 16*16th notes
}


//----------------------------------------------------------------------
//              --- MIDI Handlers ---
//----------------------------------------------------------------------

long _clockCounter = 0;

// Each time when a MIDI clock signal arrives, the 
// counter is incremented
/*void HandleClock() {

  Sync();

}*/

int bpm = (1/(duration/2)) * 0.02;

void noteDelay(int time) {
  for(int x = 0; x < time; x++) {
    delay(1);
    duration = readTempo();
    if(!playbackState){
       return;
    }
  }
}

void sendMidiNote (byte midiCommand, byte noteValue, byte velocityValue){
Serial.write(midiCommand);
Serial.write(noteValue);
Serial.write(velocityValue);
}

void setup() {
  if(octaveRange == 255 || octaveRange == 0) {octaveRange = 3;}
  clkInit();
  Serial.begin(31250);
  //LCD init
  pinMode(LCD_VCC, OUTPUT);
  pinMode(LCD_GND, OUTPUT);
  pinMode(statusPin, OUTPUT);
  //pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_VCC, HIGH);
  //digitalWrite(LCD_BL, HIGH);
  digitalWrite(LCD_GND, LOW);
  
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.setCursor(15, 15);
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.drawLine(0, 0, 83, 0, BLACK);
  display.drawLine(0, 0, 0, 45, BLACK);
  display.drawLine(83, 0, 83, 45, BLACK);
  display.drawLine(0, 45, 83, 45, BLACK);
  display.println((1/(readTempo()/2)) * 1000 * 0.02, DEC);
  display.display();
  
  //Motherboard init
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  
  attachInterrupt(1, _startStop, CHANGE); //Start/stop: pin3, int.1
  attachInterrupt(0, _restart, CHANGE); //Reset: pin2, int.0
  attachInterrupt(5, selectClockSource, CHANGE); //Reset: pin18, int.5
}

void _startStop() {
  if(!midiClock){
    startStop();
  }
}

void _restart() {
  if(!midiClock){
    restart();
  }
}

void startStop() {
  bouncerStartStop.update();
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
    playbackState = !playbackState;
    menuLevel = 1000;
    //Last note is switched off
    //sendNote(144, lastValue, 0);
    sendNoteOff(lastValue,0,1);
  }
  last_interrupt_time = interrupt_time;
}

void selectClockSource() {
  if(digitalRead(clockSourceSelectToggle) == LOW) {
     sendMidiNote (128, note, 127); //last note off
     digitalWrite(last_led, LOW);
     midiClock = false;
     playbackState = true;
     if(midiDisplayInit) {
      midiDisplayInit = false;
    }
  }
  else {
     playbackState = false;
     midiClock = true;
  }
}

void midiPulseOn() {
  digitalWrite(statusPin, HIGH);
}

void midiPulseOff() {
  digitalWrite(statusPin, LOW);
}

void restart() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
    digitalWrite(leds[stepID], LOW);
    stepID = 0;
    //Last note is switched off
    //sendNote(144, lastValue, 0);
    sendNoteOff(lastValue,0,1);
  }
  last_interrupt_time = interrupt_time;
}

int readTempo(){
  amux.SelectPin(0); // choose the pin you want to send signal to off the DeMux
  //return (map(amux.AnalogRead(), 0, 1023, minPeriod, maxPeriod));
  return (map(amux.AnalogRead(), 130, 1018, minPeriod, maxPeriod));
}

int readPot13(){
  amux.SelectPin(1); // choose the pin you want to send signal to off the DeMux
  return map((1023 - amux.AnalogRead()), 0, 1023, 0, 127);
}

int readPot(int potID) {
  if(potID == 15) {
    return readPot13();
  }
  else {
    return map((1023 - analogRead(pots[potID])), 0, 1023, 0, 127);
  }
}

/*int readTempo() {
  return (maxPeriod - map((1023 - analogRead(A13)), 0, 1023, minPeriod, maxPeriod));
}

int readPot13() {
  //return (maxPeriod - map((1023 - analogRead(A13)), 0, 1023, minPeriod, maxPeriod));
  return 0;
}*/

int readNote(int noteValue) {
  return map(noteValue, 0, 127, LowestNote, HighestNote);
}

void sendNote(int cmd, int pitch, int velocity)  {
  //sendNoteOn(pitch,velocity,1);
}

void noteOn(int stepValue, int sID) {
  if(stepValue > 0) {
    sendNoteOn(stepValue,127,1);
    analogWrite(PWMPin, stepValue/8); 
    digitalWrite(leds[sID], HIGH);
    noteDelay(duration/2);
}
  
}

void noteOff(int stepValue, int sID) {
  if(stepValue > 0) {
    //sendNote(144, readNote(stepValue), 0);
    sendNoteOff(stepValue,0,1);
    analogWrite(PWMPin, 0);
    digitalWrite(leds[sID], LOW);
    noteDelay(duration/2);
  }
}

void _nOn(int stepValue, int sID) {
  if(stepValue > 0) {
    //sendNoteOn(stepValue,127,1);
    analogWrite(PWMPin, stepValue/8); 
    digitalWrite(leds[sID], HIGH);
}
  
}

void _nOff(int stepValue, int sID) {
  if(stepValue > 0) {
    //sendNote(144, readNote(stepValue), 0);
    //sendNoteOff(stepValue,0,1);
    analogWrite(PWMPin, 0);
    digitalWrite(leds[sID], LOW);
  }
}

void serialEvent() {
    if(Serial.available() > 0) {
data = Serial.read();
if(data == midi_start) {
play_flag = 1;
clock_step=0;
}
else if(data == midi_continue) {
play_flag = 1;
}
else if(data == midi_stop) {
play_flag = 0;
clock_step=0;
}
else if((data == midi_clock) && (play_flag == 1)) {
Sync();
}
}
}

void upddisplay() {
    display.setCursor(0, 30);
    display.setTextColor(WHITE, BLACK);
    display.print("Step ID: ");
    if(_beatCount < 10) {display.print(0, DEC);}
    display.print(_beatCount, DEC);
    display.setCursor(0, 40);
    display.setTextColor(WHITE, BLACK);
    display.print("Step V.: ");
    if(lastValue < 10) {display.print("00");} else if(lastValue > 10 && lastValue < 100) {display.print("0");}
    display.print(lastValue, DEC);
    display.display();
}

void clkStop() {
  //noteOffCLK(lastValue, stepID);
  tickID = false;
}

//Menu start


//Menu item 1 END

void loop() {
  if(playbackState && !midiClock) {
   analogWrite(PWMPin, 0);
  //Last note is switched off
  //digitalWrite(leds[stepID], LOW);
  //sendNote(144, lastValue, 0);
  sendNoteOff(lastValue,0,1);
  
  duration = readTempo();
  lastValue = readPot(stepID);
  //Note on
  noteOn(lastValue, stepID);
  
  //Note off
  noteOff(lastValue, stepID);
  
  stepID++;
  
  if(stepID == 15) {
    //Read from multiplexer, then reset
    lastValue = readPot13();
    //Note on
    noteOn(lastValue, stepID);
  
    //Note off
    noteOff(lastValue, stepID);
    
    stepID = 0;
  }
  
  }
  else if(!playbackState && !midiClock)
  {
    bouncerUp.update();
    bouncerDown.update();
    bouncerEnter.update();
    //Display
    menuItem1();
  }
  else if(!playbackState && midiClock)
  {
    if(!midiDisplayInit) {
      display.clearDisplay();
      display.setCursor(0, 5);
      display.setTextSize(2);
      display.setTextColor(BLACK);
      display.println("MIDI");
      display.println("Clock");
      display.display();
      midiDisplayInit = true;
    }
  }

}

