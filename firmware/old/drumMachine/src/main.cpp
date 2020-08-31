/*
 * First Drum Machine Test
 *
 *  6.18.20
 *
 *  Dylan Reimer
 */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MIDI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#include "enc.h"
#include "synth.h"

/*
// GUItool: begin automatically generated code
AudioSynthSimpleDrum     drum2;          //xy=181,329
AudioSynthSimpleDrum     drum3;          //xy=184,387
AudioSynthSimpleDrum     drum1;          //xy=185,271
AudioSynthWaveformSine   sine1;          //xy=188,445
AudioMixer4              mixer1;         //xy=371,327
AudioOutputAnalog        dac1;           //xy=535,324
AudioConnection          patchCord1(drum2, 0, mixer1, 1);
AudioConnection          patchCord2(drum3, 0, mixer1, 2);
AudioConnection          patchCord3(drum1, 0, mixer1, 0);
AudioConnection          patchCord4(sine1, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, dac1);
// GUItool: end automatically generated code
*/

#define SW_L    14
#define ENC_L1  15
#define ENC_L2  16
#define SW_R    17
#define ENC_R1  18
#define ENC_R2  19

Encoder ENC_L = Encoder(SW_L, ENC_L1, ENC_L2);
Encoder ENC_R = Encoder(SW_R, ENC_R1, ENC_R2);

// If using software SPI (the default case):
#define OLED_MOSI  11
#define OLED_CLK   13
#define OLED_DC     6
#define OLED_CS    10
#define OLED_RESET  7
Adafruit_SH1106 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif


//AUDIO
unsigned char *sp = score;

#define AMPLITUDE (0.2)

// Create 16 waveforms, one for each MIDI channel
AudioSynthWaveform sine0, sine1, sine2, sine3;
AudioSynthWaveform sine4, sine5, sine6, sine7;
AudioSynthWaveform sine8, sine9, sine10, sine11;
AudioSynthWaveform sine12, sine13, sine14, sine15;
AudioSynthWaveform *waves[16] = {
  &sine0, &sine1, &sine2, &sine3,
  &sine4, &sine5, &sine6, &sine7,
  &sine8, &sine9, &sine10, &sine11,
  &sine12, &sine13, &sine14, &sine15
};

// allocate a wave type to each channel.
// The types used and their order is purely arbitrary.
short wave_type[16] = {
  WAVEFORM_SINE,
  WAVEFORM_SQUARE,
  WAVEFORM_SAWTOOTH,
  WAVEFORM_TRIANGLE,
  WAVEFORM_SINE,
  WAVEFORM_SQUARE,
  WAVEFORM_SAWTOOTH,
  WAVEFORM_TRIANGLE,
  WAVEFORM_SINE,
  WAVEFORM_SQUARE,
  WAVEFORM_SAWTOOTH,
  WAVEFORM_TRIANGLE,
  WAVEFORM_SINE,
  WAVEFORM_SQUARE,
  WAVEFORM_SAWTOOTH,
  WAVEFORM_TRIANGLE
};

// Each waveform will be shaped by an envelope
AudioEffectEnvelope env0, env1, env2, env3;
AudioEffectEnvelope env4, env5, env6, env7;
AudioEffectEnvelope env8, env9, env10, env11;
AudioEffectEnvelope env12, env13, env14, env15;
AudioEffectEnvelope *envs[16] = {
  &env0, &env1, &env2, &env3,
  &env4, &env5, &env6, &env7,
  &env8, &env9, &env10, &env11,
  &env12, &env13, &env14, &env15
};

// Route each waveform through its own envelope effect
AudioConnection patchCord01(sine0, env0);
AudioConnection patchCord02(sine1, env1);
AudioConnection patchCord03(sine2, env2);
AudioConnection patchCord04(sine3, env3);
AudioConnection patchCord05(sine4, env4);
AudioConnection patchCord06(sine5, env5);
AudioConnection patchCord07(sine6, env6);
AudioConnection patchCord08(sine7, env7);
AudioConnection patchCord09(sine8, env8);
AudioConnection patchCord10(sine9, env9);
AudioConnection patchCord11(sine10, env10);
AudioConnection patchCord12(sine11, env11);
AudioConnection patchCord13(sine12, env12);
AudioConnection patchCord14(sine13, env13);
AudioConnection patchCord15(sine14, env14);
AudioConnection patchCord16(sine15, env15);

// Four mixers are needed to handle 16 channels of music
AudioMixer4     mixer1;
AudioMixer4     mixer2;
AudioMixer4     mixer3;
AudioMixer4     mixer4;

// Mix the 16 channels down to 4 audio streams
AudioConnection patchCord17(env0, 0, mixer1, 0);
AudioConnection patchCord18(env1, 0, mixer1, 1);
AudioConnection patchCord19(env2, 0, mixer1, 2);
AudioConnection patchCord20(env3, 0, mixer1, 3);
AudioConnection patchCord21(env4, 0, mixer2, 0);
AudioConnection patchCord22(env5, 0, mixer2, 1);
AudioConnection patchCord23(env6, 0, mixer2, 2);
AudioConnection patchCord24(env7, 0, mixer2, 3);
AudioConnection patchCord25(env8, 0, mixer3, 0);
AudioConnection patchCord26(env9, 0, mixer3, 1);
AudioConnection patchCord27(env10, 0, mixer3, 2);
AudioConnection patchCord28(env11, 0, mixer3, 3);
AudioConnection patchCord29(env12, 0, mixer4, 0);
AudioConnection patchCord30(env13, 0, mixer4, 1);
AudioConnection patchCord31(env14, 0, mixer4, 2);
AudioConnection patchCord32(env15, 0, mixer4, 3);

// Now create 2 mixers for the main output
AudioMixer4     mixerLeft;
AudioMixer4     mixerRight;
AudioOutputAnalog        dac1;           //xy=591,313

// Mix all channels to both the outputs
AudioConnection patchCord33(mixer1, 0, mixerLeft, 0);
AudioConnection patchCord34(mixer2, 0, mixerLeft, 1);
AudioConnection patchCord35(mixer3, 0, mixerLeft, 2);
AudioConnection patchCord36(mixer4, 0, mixerLeft, 3);
AudioConnection patchCord37(mixer1, 0, mixerRight, 0);
AudioConnection patchCord38(mixer2, 0, mixerRight, 1);
AudioConnection patchCord39(mixer3, 0, mixerRight, 2);
AudioConnection patchCord40(mixer4, 0, mixerRight, 3);
AudioConnection patchCord41(mixerLeft, 0, dac1, 0);
AudioConnection patchCord42(mixerRight, 0, dac1, 1);

AudioControlSGTL5000 codec;

// Initial value of the volume control
int volume = 50;


void setup() {

  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC);
  display.display();
  delay(700);
  display.clearDisplay();

  //AUDIO
  delay(200);
// http://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
  Serial.print("Begin ");
  Serial.println(__FILE__);

  // Proc = 12 (13),  Mem = 2 (8)
  // Audio connections require memory to work.
  // The memory usage code indicates that 10 is the maximum
  // so give it 12 just to be sure.
  AudioMemory(18);

  codec.enable();
  codec.volume(0.45);

  // reduce the gain on some channels, so half of the channels
  // are "positioned" to the left, half to the right, but all
  // are heard at least partially on both ears
  mixerLeft.gain(1, 0.36);
  mixerLeft.gain(3, 0.36);
  mixerRight.gain(0, 0.36);
  mixerRight.gain(2, 0.36);

  // set envelope parameters, for pleasing sound :-)
  for (int i=0; i<16; i++) {
    envs[i]->attack(9.2);
    envs[i]->hold(2.1);
    envs[i]->decay(31.4);
    envs[i]->sustain(0.6);
    envs[i]->release(84.5);
    // uncomment these to hear without envelope effects
    //envs[i]->attack(0.0);
    //envs[i]->hold(0.0);
    //envs[i]->decay(0.0);
    //envs[i]->release(0.0);
  }

  Serial.println("setup done");

  // Initialize processor and memory measurements
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();
}



unsigned long last_time = millis();
void loop() {

  // RESET
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);

  // DIRECTIONS
  display.setTextSize(1);
  display.setCursor(1, 5);
  display.print(ENC_L.dir());
  display.setCursor(105, 5);
  display.print(ENC_R.dir());

  // VALUES
  display.setTextSize(3);
  display.setCursor((display.width() / 2) - 25, 20);
  display.print(ENC_L.read());
  display.setCursor((display.width() / 2) + 10, 20);
  display.print(ENC_R.read());

  display.display();

  // SWITCHES
  if (ENC_L.fell()) {
    display.setTextSize(1);
    display.setCursor(1, 50);
    display.print("click");
    display.display();
    delay(70);
    display.clearDisplay();
  }

  if (ENC_R.fell()) {
    display.setTextSize(1);
    display.setCursor(98, 50);
    display.printf("click");
    display.display();
    delay(70);
    display.clearDisplay();
  }


  //AUDIO
  unsigned char c,opcode,chan;
  unsigned long d_time;

// Change this to if(1) for measurement output every 5 seconds
if(1) {
  if(millis() - last_time >= 5000) {
    Serial.print("Proc = ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" (");
    Serial.print(AudioProcessorUsageMax());
    Serial.print("),  Mem = ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" (");
    Serial.print(AudioMemoryUsageMax());
    Serial.println(")");
    last_time = millis();
  }
}

  // Volume control
  //  uncomment if you have a volume pot soldered to your audio shield

  // read the next note from the table
  c = *sp++;
  opcode = c & 0xF0;
  chan = c & 0x0F;

  if(c < 0x80) {
    // Delay
    d_time = (c << 8) | *sp++;
    delay(d_time);
    return;
  }
  if(*sp == CMD_STOP) {
    for (chan=0; chan<10; chan++) {
      envs[chan]->noteOff();
      waves[chan]->amplitude(0);
    }
    Serial.println("DONE");
    while(1);
  }

  // It is a command

  // Stop the note on 'chan'
  if(opcode == CMD_STOPNOTE) {
    envs[chan]->noteOff();
    return;
  }

  // Play the note on 'chan'
  if(opcode == CMD_PLAYNOTE) {
    unsigned char note = *sp++;
    unsigned char velocity = *sp++;
    AudioNoInterrupts();
    waves[chan]->begin(AMPLITUDE * velocity2amplitude[velocity-1],
                       tune_frequencies2_PGM[note],
                       wave_type[chan]);
    envs[chan]->noteOn();
    AudioInterrupts();
    return;
  }

  // replay the tune
  if(opcode == CMD_RESTART) {
    sp = score;
    return;
  }
}
