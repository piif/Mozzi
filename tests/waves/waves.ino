#define AUDIO_MODE HIFI
#include <MozziGuts.h>
#include <Oscil.h>
#include <mozzi_midi.h>

#include <ADSR.h>

#include <PS2KeyAdvanced.h>

#include <tables/sin1024_int8.h>
Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> aSin0(SIN1024_DATA);
#include <tables/triangle512_int8.h>
Oscil <TRIANGLE512_NUM_CELLS, AUDIO_RATE> aSin1(TRIANGLE512_DATA);

//#include <tables/chum78_int8.h>
//Oscil <CHUM78_NUM_CELLS, AUDIO_RATE> aSin1(CHUM78_DATA);
//#include <tables/square_analogue512_int8.h>
//Oscil <SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aSin2(SQUARE_ANALOGUE512_DATA);
//#include <tables/pinknoise8192_int8.h>
//Oscil <PINKNOISE8192_NUM_CELLS, AUDIO_RATE> aSin3(PINKNOISE8192_DATA);
//#include <tables/saw1024_int8.h>
//Oscil <SAW1024_NUM_CELLS, AUDIO_RATE> aSin3(SAW1024_DATA);
//#include <tables/chum9_int8.h>
//Oscil <CHUM9_NUM_CELLS, AUDIO_RATE> aSin4(CHUM9_DATA);

//#include <tables/triangle_analogue512_int8.h>
//Oscil <TRIANGLE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aSin1(TRIANGLE_ANALOGUE512_DATA); // le même
//#include <tables/triangle_dist_cubed_2048_int8.h>
//Oscil <TRIANGLE_DIST_CUBED_2048_NUM_CELLS, AUDIO_RATE> aSin2(TRIANGLE_DIST_CUBED_2048_DATA);  // plus clair
//#include <tables/triangle_hermes_2048_int8.h>
//Oscil <TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> aSin3(TRIANGLE_HERMES_2048_DATA); // plus étouffé
//#include <tables/triangle_valve_2048_int8.h>
//Oscil <TRIANGLE_VALVE_2048_NUM_CELLS, AUDIO_RATE> aSin4(TRIANGLE_VALVE_2048_DATA); // Plus aigu ?
//#include <tables/triangle_warm8192_int8.h>
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
Sample <ABOMB_NUM_CELLS, AUDIO_RATE> sample(ABOMB_DATA);

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

ADSR <CONTROL_RATE, AUDIO_RATE> envADSR;
Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> envSin(SIN1024_DATA);

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
		envADSR.setLevels(255, 150, 50, 0);
		envADSR.setTimes(100, 100, 200, 100);
		envADSR.noteOn(true);
		break;
	case 2:
		envSin.setFreq(1);
		break;
	case 3:
		envSin.setFreq(10);
		break;
	case 4:
		envSin.setFreq(10);
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
		Serial.print("bamboo ");Serial.println(currentSample);
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
//             <=A#=34  Q=B   S=C   X=C#  D=D   C=D#  F=E   G=F   B=F#  H=G   N=G#  J=A   ,=A#  K=B   L=C   :=C#  M=D   !=D#  ù=E   *=F   end
byte keyMap[] = { 0x8B, 0x41, 0x53, 0x58, 0x44, 0x43, 0x46, 0x47, 0x42, 0x48, 0x4E, 0x4A, 0x4D, 0x4B, 0x4C, 0X3D, 0x5B, 0x3E, 0x3A, 0x5C, 0x00 };

word currentKey = 0;

void updateControl() {
	switch (envelop) {
	case 1:
		if (envADSR.playing()) {
			envADSR.update();
		}
		break;
	}

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
			if (envADSR.playing()) {
				e = envADSR.next();
			} else {
				e = 0;
			}
			break;
		case 2:
		case 3:
		case 4:
			e = envSin.next();
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

	return MonoOutput::fromNBit(18, base * e + 512L * sampleValue);
}

void setup(){
	Serial.begin(115200);

	keyboard.begin(DATAPIN, IRQPIN);
	keyboard.setLock(PS2_LOCK_NUM);

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
