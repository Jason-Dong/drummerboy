
#include <Arduino.h>

#include <Audio.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>
#include <Bounce2.h>

#include "enc.h"
#include "AudioSampleData.h"

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

uint8_t incoming_byte = 0;

uint8_t encVal = 0;
uint8_t kickSelect = 1;
uint8_t snareSelect = 1;
uint8_t hhSelect = 1;
uint8_t openhhSelect = 1;

#define SW_R    17
#define ENC_R1  18
#define ENC_R2  19

Encoder ENC_R = Encoder(SW_R, ENC_R1, ENC_R2);

void updateEnc() {
  if (ENC_R.fell()) {
    encVal = (encVal + 1) % 4;
  }
  switch (encVal) {
    case 1:
      kickSelect = ENC_R.read() % 9;
      break;
    case 2:
      snareSelect = ENC_R.read() % 9;
      break;
    case 3:
      hhSelect = ENC_R.read() % 3;
      break;
    case 4:
      openhhSelect = ENC_R.read() % 3;
      break;
  }
}

void kick() {
  switch (kickSelect) {
    case 1:
      playMemKick.play(AudioSampleKick_01);
      break;
    case 2:
      playMemKick.play(AudioSampleKick_02);
      break;
    case 3:
      playMemKick.play(AudioSampleKick_03);
      break;
    case 4:
      playMemKick.play(AudioSampleKick_04);
      break;
    case 5:
      playMemKick.play(AudioSampleKick_05);
      break;
    case 6:
      playMemKick.play(AudioSampleKick_06);
      break;
    case 7:
      playMemKick.play(AudioSampleKick_07);
      break;
    case 8:
      playMemKick.play(AudioSampleKick_08);
      break;
  }
}

void snare() {
  switch (snareSelect) {
    case 1:
      playMemSnare.play(AudioSampleSnare_01);
      break;
    case 2:
      playMemSnare.play(AudioSampleSnare_02);
      break;
    case 3:
      playMemSnare.play(AudioSampleSnare_03);
      break;
    case 4:
      playMemSnare.play(AudioSampleSnare_04);
      break;
    case 5:
      playMemSnare.play(AudioSampleSnare_05);
      break;
    case 6:
      playMemSnare.play(AudioSampleSnare_06);
      break;
    case 7:
      playMemSnare.play(AudioSampleSnare_07);
      break;
    case 8:
      playMemSnare.play(AudioSampleSnare_08);
      break;
  }
}

void hh() {
  switch (hhSelect) {
    case 1:
      playMemSnare.play(AudioSampleHat_classac);
      break;
    case 2:
      playMemSnare.play(AudioSampleHat_classic);
      break;
  }
}

void openhh() {
  switch (openhhSelect) {
    case 1:
      playMemSnare.play(AudioSampleHat_large);
      break;
    case 2:
      playMemSnare.play(AudioSampleHat_open_2);
      break;
  }
}

void setup() {
  Serial.begin(57600);
  println("test");

  AudioMemory(100);

  AudioNoInterrupts();

  for (int i = 0; i < 4; i++) {
    mixer.gain(i, 1.0);
  }

  AudioInterrupts();
}

void loop() {

  // updateEnc();

  if (Serial.available() > 0) {
    // read the incoming byte:
    incoming_byte = Serial.read();


    // say what you got:
    Serial.print("I received: ");
    Serial.println(incoming_byte, DEC);


    if ((incoming_byte & 1) == 1) {
      kick();
    }

    if ((incoming_byte & 2) == 2) {
      snare();
    }

    if ((incoming_byte & 4) == 4) {
      hh();
    }

    if ((incoming_byte & 8) == 8) {
      openhh();
    }
  }
}
