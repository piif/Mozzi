/*  Example playing a sinewave at a set frequency,
    using Mozzi sonification library.

    Demonstrates the use of Oscil to play a wavetable.

    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or
    check the README or http://sensorium.github.io/Mozzi/

    Mozzi documentation/API
		https://sensorium.github.io/Mozzi/doc/html/index.html

		Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Tim Barrass 2012, CC by-nc-sa.
*/

//#define AUDIO_MODE HIFI
#include <MozziGuts.h>
#include <Oscil.h>
#include <mozzi_midi.h>

#include <ADSR.h>

#include <PS2KeyAdvanced.h>

#include <tables/sin1024_int8.h>

//#include <tables/chum78_int8.h>
//#include <tables/chum9_int8.h>
//#include <tables/square_analogue512_int8.h>
//#include <tables/pinknoise8192_int8.h>
//#include <tables/saw1024_int8.h>
//
//Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> aSin0(SIN1024_DATA);
//Oscil <CHUM78_NUM_CELLS, AUDIO_RATE> aSin1(CHUM78_DATA);
//Oscil <SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aSin2(SQUARE_ANALOGUE512_DATA);
////Oscil <PINKNOISE8192_NUM_CELLS, AUDIO_RATE> aSin3(PINKNOISE8192_DATA);
//Oscil <SAW1024_NUM_CELLS, AUDIO_RATE> aSin3(SAW1024_DATA);
//Oscil <CHUM9_NUM_CELLS, AUDIO_RATE> aSin4(CHUM9_DATA);
//
//ADSR <AUDIO_RATE, AUDIO_RATE> env1;
//Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> env2(SIN1024_DATA);
//Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> env3(SIN1024_DATA);
//Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> env4(SIN1024_DATA);

#include <tables/triangle512_int8.h>
#include <tables/triangle_analogue512_int8.h>
//#include <tables/triangle_dist_cubed_2048_int8.h>
//#include <tables/triangle_hermes_2048_int8.h>
//#include <tables/triangle_valve_2048_int8.h>
//#include <tables/triangle_warm8192_int8.h>
//
Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> aSin0(SIN1024_DATA);
Oscil <TRIANGLE512_NUM_CELLS, AUDIO_RATE> aSin1(TRIANGLE512_DATA);
//Oscil <TRIANGLE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aSin1(TRIANGLE_ANALOGUE512_DATA); // le même
//Oscil <TRIANGLE_DIST_CUBED_2048_NUM_CELLS, AUDIO_RATE> aSin2(TRIANGLE_DIST_CUBED_2048_DATA);  // plus clair
//Oscil <TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> aSin3(TRIANGLE_HERMES_2048_DATA); // plus étouffé
//Oscil <TRIANGLE_VALVE_2048_NUM_CELLS, AUDIO_RATE> aSin4(TRIANGLE_VALVE_2048_DATA); // Plus aigu ?
//Oscil <TRIANGLE_WARM8192_NUM_CELLS, AUDIO_RATE> aSin5(TRIANGLE_WARM8192_DATA); // noisy ?

#include <Sample.h>

#include <tests/bamboos/bamboo_00_1024_int8.h> // G  ?
#include <tests/bamboos/bamboo_01_1024_int8.h> // G# ?
#include <tests/bamboos/bamboo_02_1024_int8.h> // B  ?
#include <tests/bamboos/bamboo_03_1024_int8.h> // D  ?
#include <tests/bamboos/bamboo_04_1024_int8.h> // E  ?
#include <tests/bamboos/bamboo_05_1024_int8.h> // G  ?
#include <tests/bamboos/bamboo_06_1024_int8.h> // A  ?
#include <tests/bamboos/bamboo_07_1024_int8.h> // B  ?
#include <tests/bamboos/bamboo_08_1024_int8.h> // D  ?
#include <tests/bamboos/bamboo_09_1024_int8.h> // E  ?
#include <tests/bamboos/bamboo_10_1024_int8.h> // G  ?

#include <tests/waves/abomb_mod_int8.h>

const int8_t* tables[] ={
  BAMBOO_00_1024_DATA,
  BAMBOO_01_1024_DATA,
  BAMBOO_02_1024_DATA,
  BAMBOO_03_1024_DATA,
  BAMBOO_04_1024_DATA,
  BAMBOO_05_1024_DATA,
  BAMBOO_06_1024_DATA,
  BAMBOO_07_1024_DATA,
  BAMBOO_08_1024_DATA,
  BAMBOO_09_1024_DATA,
  BAMBOO_10_1024_DATA
};

Sample <1024, AUDIO_RATE> bamboo;

Sample <ABOMB_NUM_CELLS, AUDIO_RATE> sample(ABOMB_DATA);

ADSR <AUDIO_RATE, AUDIO_RATE> env1;
Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> env2(SIN1024_DATA);
Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> env3(SIN1024_DATA);
Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> env4(SIN1024_DATA);

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable

int baseFreq = 440;

byte mode = 0;

void updateMode() {
	switch (mode) {
	case 0:
		aSin0.setFreq(baseFreq);
		break;
	case 1:
		aSin1.setFreq(baseFreq);
		break;
//	case 2:
//		aSin2.setFreq(baseFreq);
//		break;
//	case 3:
//		aSin3.setFreq(baseFreq);
//		break;
//	case 4:
//		aSin4.setFreq(baseFreq);
//		break;
//	case 5:
//		aSin5.setFreq(baseFreq);
//		break;
	}
}

byte envelop = 0;

void updateEnvelop() {
	switch (envelop) {
	case 1:
		env1.noteOn();
		break;
	case 2:
		env2.setFreq(1);
		break;
	case 3:
		env3.setFreq(2);
		break;
	case 4:
		env4.setFreq(10);
		break;
	}

}

int8_t currentSample = -1;

void setSample(int8_t s) {
	currentSample = s;
	if (currentSample == 100) {
		sample.setStart(0);
		sample.setEnd(3799);
		sample.start();
		Serial.println("sample 2");
	} else if (currentSample == 101) {
		sample.setStart(3800);
		sample.setEnd(3800+5999);
		sample.start();
		Serial.println("sample 1");
	} else {
		bamboo.setTable(tables[currentSample]);
		bamboo.start();
		Serial.print("sample ");Serial.println(currentSample);
	}
}

int8_t currentNote = 0;

void setNote(int8_t n) {
	currentNote = n;
	baseFreq = mtof(currentNote);
	updateMode();
	updateEnvelop();
}

/* Keyboard constants  Change to suit your Arduino
   define pins used for data and clock from keyboard */
#define DATAPIN 4
#define IRQPIN  3

PS2KeyAdvanced keyboard;

#define FIRST_NOTE 59 // midi code for B
//              Q=B=35  S=C   X=C#  D=D   C=D#  F=E   G=F   B=F#  H=G   N=G#  J=A   ,=A#  K=B   L=C   :=C#  M=D   !=D#  ù=E   *=F   end
byte keyMap[] = { 0x41, 0x53, 0x58, 0x44, 0x43, 0x46, 0x47, 0x42, 0x48, 0x4E, 0x4A, 0x4D, 0x4B, 0x4C, 0X3D, 0x5B, 0x3E, 0x3A, 0x5C, 0x00 };

word currentKey = 0;

void updateControl() {
	if( keyboard.available() ) {
		// read the next key
		word code = keyboard.read() & 0x80FF; // filter out shift, control, caps, ...
		if (code > 0 && code != currentKey) {
			if (code & 0x8000) {
				if (code == (currentKey | 0x8000)) {
					// release currentNote
					currentNote = 0;
					currentKey = 0;
				}
			} else {
				Serial.print("code ");Serial.println(code, HEX);
				if (code == PS2_KEY_BS) {
					setSample(101);
				} else if (code == PS2_KEY_EQUAL) {
					setSample(100);
				} else if (code == PS2_KEY_0) {
					setSample(9);
				} else if (code >= PS2_KEY_1 && code <= PS2_KEY_9) {
					setSample(code - PS2_KEY_1);
				} else if (code == PS2_KEY_MINUS) {
					setSample(10);

				} else if (code >= PS2_KEY_F1 && code <= PS2_KEY_F12) {
					mode = code - PS2_KEY_F1;
					Serial.print("mode ");Serial.println(mode);
					updateMode();

				} else if (code >= PS2_KEY_KP0 && code <= PS2_KEY_KP9) {
					envelop = code - PS2_KEY_KP0;
					Serial.print("envelop ");Serial.println(envelop);
					updateEnvelop();

				} else {
					for (byte *k= keyMap; *k; k++) {
						if (code == *k) {
							currentKey = code;
							byte note = k-keyMap+FIRST_NOTE;
							Serial.println(note);
							setNote(note);
							break;
						}
					}
				}
			}
		}
	}
}

AudioOutput_t updateAudio() {
	int16_t base = 0, e = 255, sampleValue = 0;

	if (currentNote != 0) {
		switch (mode) {
		case 0:
			base = aSin0.next();
			break;
		case 1:
			base = aSin1.next();
			break;
	//	case 2:
	//		base = aSin2.next();
	//		break;
	//	case 3:
	//		base = aSin3.next();
	//		break;
	//	case 4:
	//		base = aSin4.next();
	//		break;
	//	case 5:
	//		base = aSin5.next();
	//		break;
		}

		switch (envelop) {
		case 1:
			e = env1.next();
			break;
		case 2:
			e = env2.next();
			break;
		case 3:
			e = env3.next();
			break;
		case 4:
			e = env4.next();
			break;
		}
	}

	if (currentSample == 100 || currentSample == 101) {
		sampleValue = sample.next();
		if (!sample.isPlaying()) {
			currentSample = -1;
			Serial.println("sample end");
		}
	} else if (currentSample != -1) {
		sampleValue = bamboo.next();
		if (!bamboo.isPlaying()) {
			currentSample = -1;
			Serial.println("sample end");
		}
	}

	return MonoOutput::fromNBit(17, base*e + (sampleValue<<6));
}

void setup(){
	Serial.begin(115200);
	keyboard.begin(DATAPIN, IRQPIN);

	env1.setLevels(255, 150, 50, 0);
	env1.setTimes(10, 200, 400, 100);

	bamboo.setFreq((float) BAMBOO_00_1024_SAMPLERATE / (float) BAMBOO_00_1024_NUM_CELLS);
	bamboo.setLoopingOff();
	bamboo.rangeWholeSample();

	sample.setFreq((float) ABOMB_SAMPLERATE / (float) ABOMB_NUM_CELLS);
    sample.setLoopingOff();
	sample.setTable(ABOMB_DATA);

	startMozzi(CONTROL_RATE);
	Serial.println("OK");
}

void loop(){
  audioHook(); // required here
}
