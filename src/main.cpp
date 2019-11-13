#include <Arduino.h>

#include <Led.cpp>
#include <Display.cpp>
#include <Debounce.h>

const byte statusLed       = 13;
const byte sensorInterrupt = 0;
const byte buttonInterrupt = 1;
const byte sensorPin       = 2;
const byte buttonPin       = 3;
const byte debounceDelay = 50;
volatile byte button_fl = 0;
float calibrationFactors[] = {4.5, 4.5*1000, 4.5/0.264172};
volatile int calIndex = 0;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = calibrationFactors[0];

volatile byte pulseCount;  
volatile long checkpoint;
volatile long _time = 0;
volatile int button_time = 0;
volatile byte screenMode = 0;
volatile bool isPressed = false;

float flowRate;
unsigned int flowMilliLitres;

unsigned long oldTime;

Led led(8, 9, 10);
Display* display;
Debounce button(3, 50);

void pulseCounter()
{
  pulseCount++;
}

/* 
int lastReading = HIGH;
unsigned long lastReadingTime;
int buttonState;
void buttonPress() {
  int reading = button.read();
  if (reading != lastReading) {
    if (reading == LOW) {
      lastReadingTime = millis();
    } else {
      _time = millis() - lastReadingTime;
      button_fl = 1;
    }
  }
}
*/

/* 
void handleButtonChange() {
  detachInterrupt(buttonInterrupt);
  Serial.println("Inside [Button Handler]");

  int pinState = digitalRead(buttonPin);
  if (pinState == LOW) {
    button_fl = 1;
  }
  if ((millis() - checkpoint) < debounceDelay) {
    attachInterrupt(buttonInterrupt, handleButtonChange, CHANGE);
    return;
  }

  if (pinState == LOW) {
    Serial.println("=== PRESS ===");
    
    checkpoint = millis();
  } else {
    _time = millis() - checkpoint;
    
    Serial.println("=== RELEASE ===");
    Serial.print("Millis: ");
    Serial.println(millis());
    Serial.print("Checkpoint: ");
    Serial.println(checkpoint);
    Serial.print("Time: ");
    Serial.println(_time);

    checkpoint = 0;
  }
  attachInterrupt(buttonInterrupt, handleButtonChange, CHANGE);
}
*/

//void nextScreen() {
//  detachInterrupt(buttonInterrupt);
//
//  Serial.println("Inside [nextScreen()]");
//  //display->nextScreen();
//  //display->showMenu();
//  button_fl = 1;
//  button_time = millis();
//
//  attachInterrupt(buttonInterrupt, nextScreen, CHANGE);
//}

void setup()
{
  Serial.begin(9600);
   
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, HIGH);
  
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pinMode(buttonPin, INPUT_PULLUP);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  oldTime           = 0;

  display = new Display();
  display->showMenu();
  //display = display_init();
  //display_showMenu(display);

  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  //attachInterrupt(buttonInterrupt, handleButtonChange, CHANGE);
  //attachInterrupt(buttonInterrupt, handleButtonChange, CHANGE);
}

int lastReading = HIGH;
long lastPressTime = 0;
void handleButton() {
  int reading = button.read();
  if (reading == LOW && lastReading == HIGH) {
    lastPressTime = millis();
    button_fl = 1;
  } else if (reading == HIGH && lastReading == LOW) {
    _time = millis() - lastPressTime;
  }
  lastReading = reading;

  Serial.print("[");
  Serial.print(millis());
  Serial.print("] | ");
  Serial.print(reading);
  Serial.print("Reading: ");
  Serial.print(reading);
  Serial.print(" | Last Reading: ");
  Serial.print(lastReading);
  Serial.print(" | Last Press Time: ");
  Serial.print(lastPressTime);
  Serial.print(" | Time: ");
  Serial.print(_time);
  Serial.print("\n");
}

bool menuShown = false;
void loop()
{
  handleButton();
  //buttonPress();
  if (_time > 1000) {
    _time = 0;
    screenMode ^= 1;
    menuShown = false;
    Serial.println("Screen mode changed!");
    return;
  }
  if (button_fl == 1) {
    button_fl = 0;
    if (screenMode == 0 && (millis()-oldTime) > (1000/8)) {
      display->nextScreen();
      calIndex = (calIndex+1) % 3;
    }
  }

  if (screenMode == 1) {
    display->showValue(flowRate);
  } else if (!menuShown) {
    display->showMenu();
    menuShown = true;
  }

/*
  if((millis() - oldTime) > 1000)
  { 
    detachInterrupt(sensorInterrupt);
    //detachInterrupt(buttonInterrupt);
        
    Serial.print("Pulse count: ");
    Serial.print(pulseCount);
    Serial.print(" | ");
    Serial.print("Time: ");
    Serial.print(millis()-oldTime);
    Serial.print(" | ");
    Serial.print("Calibration factor: ");
    Serial.print(calibrationFactors[calIndex]);
    Serial.print("\n");
    flowRate = pulseCount/calibrationFactors[calIndex];
    oldTime = millis();


    digitalWrite(11, LOW);
    if (pulseCount < 5) {
      led.lightGreen();
    } else if (pulseCount < 48) {
      led.lightBlue();
    } else if (pulseCount < 61) {
      led.lightRed();
    } 
    if (pulseCount > 80) {
      digitalWrite(11, HIGH);
    }

    Serial.print("Pulse count: ");
    Serial.print(pulseCount);
    Serial.print(" | ");
    Serial.print("Flow rate: ");
    Serial.print(flowRate);
    Serial.print("\n");

    pulseCount = 0;
    
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    //attachInterrupt(buttonInterrupt, handleButtonChange, CHANGE);
  }
  */
}
