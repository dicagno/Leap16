int invertedAnalogRead(int inVal) {
  return map(analogRead(inVal), 1023, 0, 0, 1023);
}

void Sync() { // play 16 x 16th notes, repeat after the cycle is finshed
clock_step = clock_step+1;
/*
Note1 = (96*1)/16;
Note2 = (96*2)/16;
Note3 = (96*3)/16;
Note4 = (96*4)/16;
Note5 = (96*5)/16;
Note6 = (96*6)/16;
Note7 = (96*7)/16;
Note8 = (96*8)/16;
Note9 = (96*9)/16;
Note10 = (96*10)/16;
Note11 = (96*11)/16;
Note12 = (96*12)/16;
Note13 = (96*13)/16;
Note14 = (96*14)/16;
Note15 = (96*15)/16;
Note16 = (96*16)/16;*/

if (clock_step==Note1){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(4);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(4, HIGH);
digitalWrite(38, LOW);
last_led = 4;
}

if (clock_step==Note2){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(5);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(5, HIGH);
digitalWrite(4, LOW);
last_led = 5;
}

if (clock_step==Note3){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(7);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(6, HIGH);
digitalWrite(5, LOW);
last_led = 6;
}

if (clock_step==Note4){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(3);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(7, HIGH);
digitalWrite(6, LOW);
last_led = 7;
}

if (clock_step==Note5){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(2);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(8, HIGH);
digitalWrite(7, LOW);
last_led = 8;
}

if (clock_step==Note6){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(6);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(9, HIGH);
digitalWrite(8, LOW);
last_led = 9;
}

if (clock_step==Note7){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(0);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(10, HIGH);
digitalWrite(9, LOW);
last_led = 10;
}

if (clock_step==Note8){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(1);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(11, HIGH);
digitalWrite(10, LOW);
last_led = 11;
}

if (clock_step==Note9){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(15);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(24, HIGH);
digitalWrite(11, LOW);
last_led = 24;
}

if (clock_step==Note10){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(14);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(26, HIGH);
digitalWrite(24, LOW);
last_led = 26;
}

if (clock_step==Note11){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(10);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(28, HIGH);
digitalWrite(26, LOW);
last_led = 28;
}

if (clock_step==Note12){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(9);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(30, HIGH);
digitalWrite(28, LOW);
last_led = 30;
}

if (clock_step==Note13){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(12);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(32, HIGH);
digitalWrite(30, LOW);
last_led = 32;
}

if (clock_step==Note14){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(8);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(34, HIGH);
digitalWrite(32, LOW);
last_led = 34;
}

if (clock_step==Note15){
sendMidiNote (128, note, 127); //last note off
noteval = invertedAnalogRead(11);
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(36, HIGH);
digitalWrite(34, LOW);
last_led = 36;
}

if (clock_step==Note16){
sendMidiNote (128, note, 127); //last note off
amux.SelectPin(1);
noteval = (1023 - amux.AnalogRead());
note = map(noteval, 0, 1023, LowestNote, HighestNote);
sendMidiNote (144, note, 127); //note of this step on
digitalWrite(38, HIGH);
digitalWrite(36, LOW);
clock_step=0;
last_led = 38;
}

else if (clock_step==SeqLength){
clock_step=0;
}
}
