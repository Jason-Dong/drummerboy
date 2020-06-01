#include <Arduino.h>
#include "Plotter.h"

//Potentiometer Pin
const int potPin = A9;

//Potentiometer Reading
int potVal = 0;

//LED Pins
const int ledB1Pin = A3;
const int ledB2Pin = A2;
const int ledB3Pin = A1;
const int ledB4Pin = A0;

//LED Outputs
int ledB1Val = 0;
int ledB2Val = 0;
int ledB3Val = 0;
int ledB4Val = 0;

Plotter p;

void setup() {
  Serial.begin(9600);

  p.Begin();
  p.AddTimeGraph( "___", 1500, "Pot", potVal);
}

void loop() {
  int sense = analogRead(potPin);
  potVal = map(sense, 0, 1023, 0, 100);

  ledB1Val = 0;
  ledB2Val = 0;
  ledB3Val = 0;
  ledB4Val = 0;

  if (potVal < 50) {
    ledB1Val = map(abs(potVal - 25), 0, 25, 255, 0);
  }

  if ((potVal > 25) && (potVal < 75)) {
    ledB2Val = map(abs(potVal - 50), 0, 25, 255, 0);
  }

  if ((potVal > 50) && (potVal < 100)) {
    ledB3Val = map(abs(potVal - 75), 0, 25, 255, 0);
  }

  if ((potVal > 75)) {
    ledB4Val = map(potVal, 75, 100, 0, 255);
  }

  analogWrite(ledB1Pin, ledB1Val);
  analogWrite(ledB2Pin, ledB2Val);
  analogWrite(ledB3Pin, ledB3Val);
  analogWrite(ledB4Pin, ledB4Val);

  // Serial.print(ledB1Val);
  // Serial.print("  ");
  // Serial.print(ledB2Val);
  // Serial.print("  ");
  // Serial.print(ledB3Val);
  // Serial.print("  ");
  // Serial.print(ledB4Val);
  // Serial.print("  ");
  // Serial.print("|");
  // Serial.print("  ");
  // Serial.println(potVal);

  p.Plot();
  delay(2);
}
