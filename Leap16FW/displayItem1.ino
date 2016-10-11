//Octaves

void menuItem1() {
  if(menuLevel != 1) {
    display.clearDisplay();
    display.setCursor(0, 5);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.print("<");
    display.setTextColor(BLACK, WHITE);
    display.print(" Octave range:");
    display.println();
    display.setCursor(0, 22);
    display.setTextColor(WHITE, BLACK);
    display.print("*");
    display.setTextColor(BLACK, WHITE);
    display.print(" Current: ");
    display.print(octaveRange, DEC);
    display.setCursor(0, 40);
    display.setTextColor(WHITE, BLACK);
    display.print(">");
    display.display();
    menuLevel = 1;
  }
  if(!editoctaveRange) {
    if(bouncerEnter.read() == LOW) {
        buttonPressedEnter = true;
      }
  
    if(bouncerEnter.read() == HIGH && buttonPressedEnter) {
      buttonPressedEnter = false;
      editoctaveRangeF();
      editoctaveRange = true;
    }
   if(bouncerUp.read() == LOW) {
      buttonPressedUp = true;
    }
    
    if(bouncerDown.read() == LOW) {
      buttonPressedDown = true;
    }
    
    if(bouncerUp.read() == HIGH && buttonPressedUp) {
      menuLevel = 1000;
      buttonPressedUp = false;
    }
    
    if(bouncerDown.read() == HIGH && buttonPressedDown) {
      //previous menu, selectedMenu--
      //none
      buttonPressedDown = false;
    }
  } else {
    if(bouncerUp.read() == LOW) {
      buttonPressedUp = true;
    }
    
    if(bouncerDown.read() == LOW) {
      buttonPressedDown = true;
    }
    
    if(bouncerEnter.read() == LOW) {
      buttonPressedEnter = true;
    }
    
    if(bouncerUp.read() == HIGH && buttonPressedUp) {
      octaveRangeUp();
      buttonPressedUp = false;
    }
    
    if(bouncerDown.read() == HIGH && buttonPressedDown) {
      octaveRangeDown();
      buttonPressedDown = false;
    }
    
    if(bouncerEnter.read() == HIGH && buttonPressedEnter) {
      EEPROM.write(120, octaveRange);
      menuLevel = 1000;
      buttonPressedEnter = false;
      menuItem1();
      editoctaveRange = false;
    }
    
  }
}

void editoctaveRangeF() {
    display.clearDisplay();
    display.setCursor(0, 5);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.print("+");
    display.setTextColor(BLACK, WHITE);
    display.print(" Octave range:");
    display.println();
    display.setCursor(0, 22);
    display.setTextColor(WHITE, BLACK);
    display.print("*");
    display.setTextColor(BLACK, WHITE);
    display.print(" Edit: ");
    display.print(octaveRange, DEC);
    display.setCursor(0, 40);
    display.setTextColor(WHITE, BLACK);
    display.print("-");
    display.display();
    menuLevel = 1;
}

void octaveRangeUp() {
  if(octaveRange < 6) {
    octaveRange = octaveRange + 1;
    display.clearDisplay();
    display.setCursor(0, 5);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.print("+");
    display.setTextColor(BLACK, WHITE);
    display.print(" Octave range:");
    display.println();
    display.setCursor(0, 22);
    display.setTextColor(WHITE, BLACK);
    display.print("*");
    display.setTextColor(BLACK, WHITE);
    display.print(" Edit: ");
    display.print(octaveRange, DEC);
    display.setCursor(0, 40);
    display.setTextColor(WHITE, BLACK);
    display.print("-");
    display.display();
    menuLevel = 1;
  }
}

void octaveRangeDown() {
  if(octaveRange > 1) {
    octaveRange = octaveRange - 1;
    display.clearDisplay();
    display.setCursor(0, 5);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.print("+");
    display.setTextColor(BLACK, WHITE);
    display.print(" Octave range:");
    display.println();
    display.setCursor(0, 22);
    display.setTextColor(WHITE, BLACK);
    display.print("*");
    display.setTextColor(BLACK, WHITE);
    display.print(" Edit: ");
    display.print(octaveRange, DEC);
    display.setCursor(0, 40);
    display.setTextColor(WHITE, BLACK);
    display.print("-");
    display.display();
    menuLevel = 1;
  }
}
