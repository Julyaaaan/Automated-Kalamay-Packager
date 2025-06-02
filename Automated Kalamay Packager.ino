#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <Servo.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0X27, 16, 2);

// Counting
int count;
int count1;
int count2;
int count3;

// Stepper motor set-up
int timeStr = 500;
int limit = 8675; // Limit 
const int pul = 2;
const int Dir = 1;

// Conveyor sensor
int SensorPin1 = A0;
int SensorPin2 = A1;
int SensorPin3 = A2;
int sensor1 = 0;
int sensor2 = 0;
int sensor3 = 0;

// Pushbutton 
int ForwardButton = 13;
int ReverseButton = 12;
int ConveyorButton = 9;
int stepperClockwise = 11;
int stepperCounterclockwise = 10;
int forw = 0;
int rev = 0;
int con = 0;
int clockwise = 0;
int counterwise = 0;

// Manual & Auto state
int AutoButton = 8;
int ManualButton = 7;

// Motor output
int ForwardPin = 6;
int ReversePin = 5;
int Conveyormotor = 3;

// Speaker output
int SpeakerPin = 4;

// Timer variables
unsigned long sensor2TriggeredTime = 0;
bool sensor2Active = false;

void setup() {
  // LCD display and count
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PACKAGING ");
  lcd.setCursor(0, 1);
  lcd.print("COUNT ");
  
  // Manual & Auto state
  pinMode(AutoButton, INPUT_PULLUP);
  pinMode(ManualButton, INPUT_PULLUP);

  // Pushbutton 
  pinMode(ConveyorButton, INPUT_PULLUP);
  pinMode(ForwardButton, INPUT_PULLUP);
  pinMode(ReverseButton, INPUT_PULLUP);
  pinMode(stepperClockwise, INPUT_PULLUP);
  pinMode(stepperCounterclockwise, INPUT_PULLUP);

  // Conveyor sensor
  pinMode(SensorPin1, INPUT);
  pinMode(SensorPin2, INPUT);
  pinMode(SensorPin3, INPUT);

  // Output
  pinMode(Conveyormotor, OUTPUT);
  pinMode(ForwardPin, OUTPUT);
  pinMode(ReversePin, OUTPUT);
  pinMode(SpeakerPin, OUTPUT);

  digitalWrite(Conveyormotor, HIGH);
  digitalWrite(ForwardPin, HIGH);
  digitalWrite(ReversePin, HIGH);

  // Stepper motor
  pinMode(pul, OUTPUT);
  pinMode(Dir, OUTPUT);
}

void forward() {
  digitalWrite(Dir, 1);
  digitalWrite(pul, 1);
  delayMicroseconds(timeStr);
  digitalWrite(pul, 0);
  delayMicroseconds(timeStr);
}

void reverse() {
  digitalWrite(Dir, 0);
  digitalWrite(pul, 1);
  delayMicroseconds(timeStr);
  digitalWrite(pul, 0);
  delayMicroseconds(timeStr);
}

void loop() {
  // Conveyor sensor
  sensor1 = analogRead(SensorPin1);
  sensor2 = analogRead(SensorPin2);
  sensor3 = analogRead(SensorPin3);

  // Auto & Manual
  int Auto = digitalRead(AutoButton);
  int Manual = digitalRead(ManualButton);

  // Pushbutton sensor
  forw = digitalRead(ForwardButton);
  rev = digitalRead(ReverseButton);
  con = digitalRead(ConveyorButton);
  clockwise = digitalRead(stepperClockwise);
  counterwise = digitalRead(stepperCounterclockwise);

  // AUTOMATIC MODE
  if (sensor1 < 100 && Auto == LOW) {
    digitalWrite(Conveyormotor, LOW);
  } 
  else if (sensor2 < 100 && count1 == 0 && Auto == LOW) {
    sensor2TriggeredTime = millis(); // Start the timer
    sensor2Active = true;
    digitalWrite(Conveyormotor, HIGH);
    delay(1000);
    for (int i = 0; i <= limit; i++) {
      forward();
    }
    tone(SpeakerPin, 1000);
    delay(2000);
    for (int x = 0; x <= limit; x++) {
      reverse();
    }
    count++;
    lcd.setCursor(7, 1);
    lcd.print(count);
    noTone(SpeakerPin);
    delay(1000);
    digitalWrite(Conveyormotor, LOW);
    delay(1200);
  } 
  // Check if sensor 3 detects something within 25 seconds
  else if (sensor2Active && sensor3 >= 100 && millis() - sensor2TriggeredTime > 25000 && Auto == LOW) {
    // No detection by sensor 3 within 15 seconds, sound the speaker
    tone(SpeakerPin, 2000); // Notify with a different tone
    delay(2000);
    digitalWrite(Conveyormotor, HIGH);
    noTone(SpeakerPin);     // Turn off the speaker
    sensor2Active = false;  // Reset the flag
  } 
    else if (sensor2Active && sensor3 <= 100 && millis() - sensor2TriggeredTime < 25000 && Auto == LOW) {
    sensor2Active = false;  // Reset the flag
  } 
  //else if (sensor3 < 100 && Auto == LOW && count1 == 0) {
    //count++;
    //lcd.setCursor(7, 1);
    //lcd.print(count);
    //delay(1000);
  //} 
  else if (count1 == 0 && count == 5 && Auto == LOW) {
    digitalWrite(ForwardPin, LOW);
    delay(1000);
    count1 = 1;
  } 
  else if (count1 == 1 && Auto == LOW && count2 == 0) {
    digitalWrite(ForwardPin, HIGH);
    count2 = 1;
  } 
  else if (sensor2 < 100 && count1 == 1 && Auto == LOW && count2 == 1) {
        sensor2TriggeredTime = millis(); // Start the timer
    sensor2Active = true;
    digitalWrite(Conveyormotor, HIGH);
    delay(1000);
    for (int i = 0; i <= limit; i++) {
      forward();
    }
    tone(SpeakerPin, 1000);
    delay(2000);
    for (int x = 0; x <= limit; x++) {
      reverse();
    }
    count++;
    lcd.setCursor(7, 1);
    lcd.print(count);
    noTone(SpeakerPin);
    delay(1000);
    digitalWrite(Conveyormotor, LOW);
    delay(1200);
  } 
  else if (count1 == 1 && count2 == 1 && count3 == 0 && count == 10 && Auto == LOW) {
    digitalWrite(ReversePin, LOW);
    delay(1000);
    count3 = 1;
  } 
  else if (count1 == 1 && count2 == 1 && count3 == 1 && Auto == LOW) {
    digitalWrite(ReversePin, HIGH);
    delay(1000);
    count = 0;
    lcd.setCursor(8, 1);
    lcd.print("  ");
    lcd.setCursor(7, 1);
    lcd.print(count);
    delay(1000);
    count1 = 0;
    count2 = 0;
    count3 = 0;
  }

  // MANUAL MODE
  // Output conveyor
  if (rev == LOW && Manual == LOW && forw == HIGH) {
    digitalWrite(ForwardPin, HIGH);
    digitalWrite(ReversePin, LOW);
  } 
  else {
    digitalWrite(ReversePin, HIGH);
  }
  if (forw == LOW && Manual == LOW && rev == HIGH) {
    digitalWrite(ReversePin, HIGH);
    digitalWrite(ForwardPin, LOW);
  } 
  else {
    digitalWrite(ForwardPin, HIGH);
  }

  // Stepper motor
  if (clockwise == LOW && Manual == LOW && counterwise == HIGH) {
    for (int i = 0; i <= limit; i++)
      forward();
  }
  if (clockwise == HIGH && Manual == LOW && counterwise == LOW) {
    for (int x = 0; x <= limit; x++)
      reverse();
  }

  // Main conveyor
  if (con == LOW && Manual == LOW) {
    digitalWrite(Conveyormotor, LOW);
  }
  if (con == HIGH && Manual == LOW) {
    digitalWrite(Conveyormotor, HIGH);
  }
}
