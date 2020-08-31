#include <Arduino.h>

#include <Audio.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Bounce2.h>

// GUItool: begin automatically generated code
AudioPlayMemory          playMemKick;       //xy=183,460
AudioPlayMemory          playMemSnare;       //xy=185,383
AudioPlayMemory          playMemHH;       //xy=195,305
AudioPlayMemory          playMemOpenHH;       //xy=196,240
AudioMixer4              mixer;         //xy=371,327
AudioOutputAnalog        dac;           //xy=591,324
AudioConnection          patchCord1(playMemKick, 0, mixer, 3);
AudioConnection          patchCord2(playMemSnare, 0, mixer, 2);
AudioConnection          patchCord3(playMemHH, 0, mixer, 1);
AudioConnection          patchCord4(playMemOpenHH, 0, mixer, 0);
AudioConnection          patchCord5(mixer, dac);
// GUItool: end automatically generated code

AudioControlSGTL5000 codec;

// Initial value of the volume control
int volume = 50;


void setup() {
}

void loop() {
}
