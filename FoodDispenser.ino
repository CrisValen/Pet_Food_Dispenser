#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long GlobalTime = 0, GlobalTimeS = 0;

bool Calib = false, MsgDone = false, Timmer = false;
int menuState = 0, SettingsMenu = 0, TimerSet = 0, AutoTime = 0, TimeUnit = 0, Time = 0; // Indicate the actual menu
String TUnit = "Seconds";
int Hall    = 9;  // Magnetic Sensor
int Menu    = 2;  // Menu
int Up      = 3;  // Up
int Down    = 4;  // Down
int Set     = 5;  // Trigger/Set
int Clock   = 10; // Clockwise
int Counter = 12; // Counterclockwise
int Enable  = 11; // Enable

void setup() {
  pinMode(Hall, INPUT);
  pinMode(Menu, INPUT);
  pinMode(Up, INPUT);
  pinMode(Down, INPUT);
  pinMode(Set, INPUT);
  pinMode(Clock, OUTPUT);
  pinMode(Counter, OUTPUT);
  pinMode(Enable, OUTPUT);

  lcd.init();      // Initialize lcd screen
  lcd.backlight(); // Turn on the backlight
}

void loop() {
  GlobalTime = millis();
  GlobalTimeS = GlobalTime / 1000;

  switch (menuState) {

    case 0:  // Start Page
      if (Calib == false) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Welcome...");
        Calibrate();
      } else if (Calib == true && MsgDone == false) {
        InitMsg();
      } else {
        digitalWrite(Clock, LOW);
        digitalWrite(Counter, LOW);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Press 'Menu' to select mode:");
               Calib = true;
        MsgDone = true;
        NavigateMenu(1, 0);
      }
      break;
    case 1: // Manual Mode
      delay(50);
      digitalWrite(Enable, HIGH);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Manual Mode - press Set to Dispense");
         NavigateMenu(2, 1);
      if (digitalRead(Set) == HIGH) { // Dispense
        Dispense();
      } else {
        digitalWrite(Clock, LOW);
        digitalWrite(Counter, LOW);
      }
      break;
    case 2: // Automatic Mode
      delay(50);
      lcd.clear();
      digitalWrite(Enable, HIGH);
      lcd.setCursor(4, 0);
      lcd.print("Auto Mode - press Set to start");
           NavigateMenu(3, 2);
      if (digitalRead(Set) == HIGH) { // Auto Dispense
        AutoDispense();
      } else {
        digitalWrite(Clock, LOW);
        digitalWrite(Counter, LOW);
      }
      break;
    case 3: // Settings
      delay(50);
      lcd.clear();
      digitalWrite(Enable, HIGH);
      lcd.setCursor(4, 0);
      lcd.print("Settings - press Set to adjust");
           NavigateMenu(1, 3);
      if (digitalRead(Set) == HIGH) { // Enter Settings Menu
        menuState = 4;
      } else {}
      break;
    //-------------------Settings Menu ---------------------------------//
    case 4:  // Set up Timer
      delay(50);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Set up Timer");
      NavigateMenu(3, 4);
      NavigateSettings(5, 6);
      if (digitalRead(Set) == HIGH) {
        Timmer = false;
        SetTimer();
      } else {}
      break;
    case 5:  // Calibrate
      delay(50);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Calibrate");
      NavigateMenu(3, 5);
      NavigateSettings(6, 4);
      if (digitalRead(Set) == HIGH) {
        Calibrate();
      } else {
        digitalWrite(Clock, LOW);
        digitalWrite(Counter, LOW);
      }
      break;
    case 6:   // Manual Rotation
      delay(50);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Manual rotation");
      NavigateMenu(3, 6);
      NavigateSettings(4, 5);
      if (digitalRead(Set) == HIGH) {
        Spin();
      } else {
        digitalWrite(Clock, LOW);
        digitalWrite(Counter, LOW);
      }
      break;
    default:
      break;
  }
}

/*-------------------------------BLOCK OF FUCTIONS------------------*/

//-------------------------Fuction to Dispense one portion------------------------//
void Dispense() {
  analogWrite(Enable, 0);
  digitalWrite(Clock, LOW);
  digitalWrite(Counter, HIGH);
  delay(2000);
  return;
}
//-------------------------Fuction to Dispense Automatically------------------------//
void AutoDispense() {
  bool Auto = true;

  if (Auto == true) {}
  analogWrite(Enable, 0);
  digitalWrite(Clock, LOW);
  digitalWrite(Counter, HIGH);
  delay(2000);
  if (digitalRead(Menu) == HIGH) {
    Auto == false;
  }
  else {
    return;
  }
}
bool Calibrate() {
  digitalWrite(Clock, LOW);
  digitalWrite(Counter, HIGH);
  if (GlobalTimeS > 5) {
    Calib = true;
    return;
  }
  else {
    Calib = false;
    return;
  }
}

void Spin() {
  digitalWrite(Clock, LOW);
  digitalWrite(Counter, HIGH);
  analogWrite(Enable, 0);
  delay(2000);
  return;
}



//----------------------Navigate Menu Screen----------------------------------------//
void NavigateMenu(int next, int current) { //Check if menu was pressed & changes to next screen
  int buttonState = digitalRead(Menu);
  if (buttonState == HIGH) {
    menuState = next;
    return;
  } else {
    menuState = current;
    return;
  }
}
//---------------------Navigate Settings Menu-----------------------------------------//
void NavigateSettings(int next, int previous) {
  int buttonState = digitalRead(Up);
  int buttonState1 = digitalRead(Down);
  if (buttonState == HIGH) {
    menuState = next;
    return;
  }
  else {}
  if (buttonState1 == HIGH) {
    menuState = previous;
    return;
  }
  else {
    return;
  }
}
//---------------------Navigate Time Menu-----------------------------------------//
void NavigateTime(int next, int previous) {
  if (digitalRead(Up) == HIGH) {
    TimeUnit = next;
  }
  else { }
  if (digitalRead(Down) == HIGH) {
    TimeUnit = previous;
  }
  else { }
  return;
}
//-------------------------------Set up time for Automatic Mode--------------------------//

void SetTimer() {
  if (Timmer == false) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("");
    delay(3000);
    Timmer = true;
  }
  while ( Timmer == true) {
    switch (TimeUnit) {
      case 0:
        delay(50);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Seconds");
        NavigateTime(1, 3);
        if (digitalRead(Set) == HIGH) {
          TUnit = "Seconds";
          TimeUnit = 4;
        } else {}
        break;
      case 1:
        delay(50);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Minutes");
        NavigateTime(2, 0);
        if (digitalRead(Set) == HIGH) {
          TUnit = "Minutes";
          TimeUnit = 4;
        } else {}
        break;
      case 2:
        delay(50);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Hours");
        NavigateTime(3, 1);
        if (digitalRead(Set) == HIGH) {
          TUnit = "Hours";
          TimeUnit = 4;
        } else {}
        break;
      case 3:
        delay(50);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Days");
        NavigateTime(0, 2);
        if (digitalRead(Set) == HIGH) {
          TUnit = "Days";
          TimeUnit = 4;
        } else {}
        break;
      case 4:
        delay(50);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Use the ARROOWS to adjust the time");
        if (digitalRead(Up) == HIGH) {
          Time = Time + 1;
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print(Time);
          lcd.setCursor(4, 10);
          lcd.print(TUnit);
        }
        if (digitalRead(Down) == HIGH) {
          Time =  Time - 1;
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print(Time);
          lcd.setCursor(4, 10);
          lcd.print(TUnit);
        }
        if (digitalRead(Set) == HIGH) {
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("TIMER SET");
          Timmer = false;
          delay(50);
          return;
        }
        break;
      default:
        break;
    }
  }

}

//----------------------Initial Msg------------------------------------------//
void InitMsg() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Please read Carefully:");
  delay(2000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Use 'Menu' to navigate the modes");
  delay(3000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Once in the desired mode press 'SET'");
  delay(3000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("On the settings menu use the 'ARROWS' to navigate");
  delay(3000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("To access any option press 'SET'");
  delay(3000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("To go back press 'MENU'");
  delay(3000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Press 'Menu' to select mode:");
  delay(3000);
  MsgDone = true;
  return;
}
