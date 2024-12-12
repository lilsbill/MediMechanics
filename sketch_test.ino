#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <Servo.h>

Servo myServo;  // Create a Servo object

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize with I2C address, 16 columns, 2 rows

const int sensorPin = 2;  // Digital pin connected to the beam break sensor
int sensorState = 0;


const int buzzer = 7;  //buzzer to arduino pin 9

int loops1 = 0;

int seconds = 0;
int numInput = 0;
int finalday = 0;
int finalhour = 0;
int finalamPM = 0;
int finalmin = 0;

int phase = 0;


int hours = 1;
int mins = 0;
int finalHour = 0;
int finalMin = 0;

// Button states
int lastButtonState1 = HIGH;
int lastButtonState2 = HIGH;
int lastButtonState3 = HIGH;
int lastButtonState4 = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;  // 50ms debounce time

struct allData {
  int day;
  int amPm;
  int hour;
  int min;
};

allData arrayData[10];

int pill_counter = 0;
int arrowPosition = 1;


String weekDay[] = { "", "Monday", "", "Tuesday", "", "Wednesday", "", "Thursday", "", "Friday", "", "Saturday", "", "Sunday" };

byte upArrow[8] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00000
};

byte upArrow1[8] = {
  B00001,
  B00011,
  B00111,
  B01111,
  B11111,
  B00001,
  B00001,
  B00001
};

byte upArrow2[8] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11111,
  B10000,
  B10000,
  B10000
};

void updateArrowPosition() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("M T W T F S S");
  lcd.setCursor(arrowPosition, 1);
  lcd.write(byte(0));
}

void dayOutput(int day_Var) {
  switch (day_Var) {
    case 1: Serial.println("Monday"); break;
    case 2: Serial.println("Tuesday"); break;
    case 3: Serial.println("Wednesday"); break;
    case 4: Serial.println("Thursday"); break;
    case 5: Serial.println("Friday"); break;
    case 6: Serial.println("Saturday"); break;
    case 7: Serial.println("Sunday"); break;
  }
}

void setTime() {
  int timeDay = 1;

  lcd.setCursor(4, 0);
  lcd.print("Set Time");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    AM    PM");

  lcd.setCursor(4, 1);
  lcd.write(byte(1));
  lcd.setCursor(5, 1);
  lcd.write(byte(2));

  int loops = 0;

  while (loops != 1) {
    int button1State = digitalRead(A0);
    int button2State = digitalRead(A1);
    int button3State = digitalRead(A2);
    int button4State = digitalRead(A3);

    if (button1State == LOW && lastButtonState1 == HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("    AM    PM");


      lcd.setCursor(10, 1);
      lcd.write(byte(1));
      lcd.setCursor(11, 1);
      lcd.write(byte(2));
      timeDay = 2;
    }

    if (button2State == LOW && lastButtonState2 == HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("    AM    PM");

      lcd.setCursor(4, 1);
      lcd.write(byte(1));
      lcd.setCursor(5, 1);
      lcd.write(byte(2));
      timeDay = 1;
    }

    if (button3State == LOW && lastButtonState3 == HIGH) {
      loops = 1;
      delay(1000);
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print(timeDay == 1 ? "AM Selected" : "PM Selected");
      delay(2000);
    }

    lastButtonState1 = button1State;
    lastButtonState2 = button2State;
    lastButtonState3 = button3State;
  }

  finalamPM = timeDay;
  setHour();
}

void setHour() {
  int hours = 1;
  int loops = 0;
  int mins = 0;
  int loops1;

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("1 : 00");

  lcd.setCursor(5, 1);
  lcd.write(byte(0));

  delay(1000);

  while (loops != 1) {
    int button1State = digitalRead(A0);
    int button2State = digitalRead(A1);
    int button3State = digitalRead(A2);
    int button4State = digitalRead(A3);

    if (button1State == LOW && lastButtonState1 == HIGH) {
      if (hours < 12) {
        lcd.setCursor(0, 0);
        lcd.print("                ");

        hours = hours + 1;
        lcd.setCursor(5, 0);
        lcd.print(hours);
        lcd.print(" : 00");
      }
    }

    if (button2State == LOW && lastButtonState2 == HIGH) {
      if (hours > 1) {
        lcd.setCursor(0, 0);
        lcd.print("                ");

        hours = hours - 1;
        lcd.setCursor(5, 0);
        lcd.print(hours);
        lcd.print(" : 00");
      }
    }

    if (button3State == LOW && lastButtonState3 == HIGH) {
      loops = 1;
      lcd.setCursor(0, 1);
      lcd.print("                ");

      lcd.setCursor(10, 1);
      lcd.write(byte(0));

      finalhour = hours;

      delay(1000);

      // set time of day
    }

    lastButtonState1 = button1State;
    lastButtonState2 = button2State;
    lastButtonState3 = button3State;
  }

  loops1 = 0;
  while (loops1 != 1) {
    int button1State = digitalRead(A0);
    int button2State = digitalRead(A1);
    int button3State = digitalRead(A2);
    int button4State = digitalRead(A3);

    // Increment minutes
    if (button1State == LOW && lastButtonState1 == HIGH) {
      if (mins < 60) {
        mins = (mins < 55) ? mins + 5 : mins + 1;  // Add 5 if < 55, otherwise add 1
        if (mins > 60) mins = 60;                  // Cap at 60

        // Clear only the relevant display line
        lcd.setCursor(0, 0);
        lcd.print("                ");  // Clear the first line

        // Reprint updated time
        lcd.setCursor(5, 0);
        lcd.print(hours);
        lcd.print(" : ");
        if (mins < 10) {
          lcd.print("0");
        }
        lcd.print(mins);
      }
    }

    // Decrement minutes
    if (button2State == LOW && lastButtonState2 == HIGH) {
      if (mins > 0) {
        mins = (mins > 5) ? mins - 5 : mins - 1;  // Subtract 5 if > 5, otherwise subtract 1
        if (mins < 0) mins = 0;                   // Cap at 0

        // Clear only the relevant display line
        lcd.setCursor(0, 0);
        lcd.print("                ");  // Clear the first line

        // Reprint updated time
        lcd.setCursor(5, 0);
        lcd.print(hours);
        lcd.print(" : ");
        if (mins < 10) {
          lcd.print("0");
        }
        lcd.print(mins);
      }
    }

    // Confirm and exit loop
    if (button3State == LOW && lastButtonState3 == HIGH) {
      loops1 = 1;  // Exit loop
    }

    // Update button states
    lastButtonState1 = button1State;
    lastButtonState2 = button2State;
    lastButtonState3 = button3State;
  }
}


void setDay(int input) {
  if (input == 1) {
    arrowPosition = min(arrowPosition + 2, 15);
    updateArrowPosition();
  } else if (input == 2 && arrowPosition != 1) {
    arrowPosition = max(arrowPosition - 2, 1);
    updateArrowPosition();
  } else if (input == 3) {
    lcd.clear();
    lcd.print(weekDay[arrowPosition]);
    lcd.print(" Select");
    finalday = arrowPosition;
    delay(2000);
    lcd.clear();
    setTime();
  }
}

void setup() {
  pinMode(sensorPin, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);

  myServo.attach(9);
  lcd.clear();
  lcd.init();

  lcd.begin(16, 2);
  lcd.backlight();
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);

  lcd.createChar(0, upArrow);
  lcd.createChar(1, upArrow1);
  lcd.createChar(2, upArrow2);

  lcd.setCursor(1, 0);
  lcd.print("M T W T F S S");
  lcd.setCursor(arrowPosition, 1);
  lcd.write(byte(0));  // Display up arrow
}

void loop() {

  int button1State = digitalRead(A0);
  int button2State = digitalRead(A1);
  int button3State = digitalRead(A2);
  int button4State = digitalRead(A3);

  if (button1State == LOW && lastButtonState1 == HIGH) {
    setDay(1);
  } else if (button2State == LOW && lastButtonState2 == HIGH) {
    setDay(2);

  } else if (button3State == LOW && lastButtonState3 == HIGH) {
    //setHour();
    setDay(3);
    numInput++;
    //arrayData[numInput] = {finalday, finalhour, finalamPM, finalmin};
    lcd.clear();

    //lcd.print(arrayData[numInput].day);
    //lcd.print(arrayData[numInput].amPm);
    //lcd.print(arrayData[numInput].hour);
    //lcd.print(arrayData[numInput].min);

    lcd.print("Date and Time");
    lcd.setCursor(0, 1);
    lcd.print("Saved");


    delay(2000);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("M T W T F S S");
    lcd.setCursor(arrowPosition, 1);
    lcd.write(byte(0));  // Display up arrow
  } else if (button4State == LOW && lastButtonState4 == HIGH) {
    for (int pos = 5; pos <= 60; pos += 1) {
      myServo.write(pos);
      delay(20);  // Adjust delay as needed for smoother or faster movement
    }

    for (int pos = 60; pos >= 5; pos -= 1) {
      myServo.write(pos);
      delay(20);  // Adjust delay as needed for smoother or faster movement
    }

    //sensorState = digitalRead(sensorPin);
    int pill = 0;
    int pill_check = 0;

    while (pill != 1) {
      sensorState = digitalRead(sensorPin);

      if (sensorState == LOW) {
        lcd.clear();
        lcd.print("Pill dispensed");

        tone(buzzer, 1000);  // Send 1KHz sound signal...


        pill_check = 1;

        delay(1000);

      }

      else {
        lcd.clear();
        //lcd.print("Pill");

        if (pill_check == 1) {
          lcd.clear();
          lcd.print("Pill taken");

          noTone(buzzer);

          pill = 1;
        }
      }

      delay(1000);
    }
    // Small delay for stability

    delay(2000);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("M T W T F S S");
    lcd.setCursor(arrowPosition, 1);
    lcd.write(byte(0));  // Display up arrow
  }

  lastButtonState1 = button1State;
  lastButtonState2 = button2State;
  lastButtonState3 = button3State;
  lastButtonState4 = button4State;
  delay(100);
}
