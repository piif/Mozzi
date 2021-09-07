#include <Arduino.h>

#define DEBUG(args) args

#define AUDIO_MODE HIFI // MUST BE MODIFIED IN mozzi_config.h
#include <MozziGuts.h>
#include <Oscil.h>
#include <mozzi_midi.h>

#include <ADSR.h>

#include <PS2KeyAdvanced.h>

#define NB_HARMONICS 5
#define NB_TABLES 5

typedef struct __attribute__((__packed__)) _harmonics {
	byte ratios[NB_HARMONICS];
	byte bitShift;
} Harmonics;

Harmonics harmonicTables[NB_TABLES] = {
	{ {  1,  0,  0,  0,  0/*,  0*/ }, 0 }, // simple sinusoïde
	{ {  8,  4,  0,  2,  0/*,  1*/ }, 4 }, // square
	{ { 16,  7,  5,  1,  2/*,  0*/ }, 5 }, // 100, 30, 30, 10, 15 : piano ?
	{ {  8, 16, 12,  6,  6/*,  0*/ }, 6 }, // 50%, 100, 75, 40, 40: trombone ?
	{ { 20, 17, 13,  9,  6/*,  0*/ }, 7 }, // trumpet ?
};

#include <tables/sin1024_int8.h>
Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> harmonicOscils[NB_HARMONICS];

byte currentHarmonic = 0;

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
//  BAMBOO_10_1024_DATA
};
Sample <1024, AUDIO_RATE> bamboo;

#include <tests/waves/abomb_mod_int8.h>
Sample <ABOMB_NUM_CELLS, AUDIO_RATE> sample(ABOMB_DATA);

ADSR <CONTROL_RATE, AUDIO_RATE> envADSR;
Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> envSin(SIN1024_DATA);

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable

int baseFreq = 440;

void updateMode() {
	// nothing to do ?
}

byte currentEnvelop = 0;

void updateEnvelop() {
	switch (currentEnvelop) {
	case 1:
		envADSR.setLevels(255, 150, 50, 0);
		envADSR.setTimes(20, 100, 200, 100);
		envADSR.noteOn(true);
		break;
	case 2:
		envADSR.setLevels(255, 180, 150, 0);
		envADSR.setTimes(10, 200, 500, 300);
		envADSR.noteOn(true);
		break;
	case 3:
		envSin.setFreq(1);
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
		DEBUG( Serial.println("sample tom"); )
	} else if (currentSample == 101) {
		sample.setStart(3800);
		sample.setEnd(3800+5999);
		sample.start();
		DEBUG( Serial.println("sample clear"); )
	} else {
		bamboo.setTable(tables[currentSample]);
		bamboo.start();
		DEBUG( Serial.print("bamboo ");Serial.println(currentSample); )
	}
}

int8_t currentNote = 0;

void setNote(int8_t n) {
	currentNote = n;
	baseFreq = mtof(currentNote);
	for(byte i = 0; i< NB_HARMONICS; i++) {
		harmonicOscils[i].setFreq(baseFreq * (i+1));
	}
	updateEnvelop();
}

typedef struct __attribute__((__packed__)) _recordEntry {
	byte key;
	word timestamp;
} RecordEntry;

#define MAX_RECORD_ENTRIES 20
#define MAX_RECORDS 5

typedef struct _record {
	byte loop:1;
	byte harmonic:3;
	byte envelop:3;
	byte length;
	RecordEntry entries[MAX_RECORD_ENTRIES];
} Record;

Record record[MAX_RECORDS] = { { 0, 0, 0, 0, { 0, } } };
Record *currentRecord = NULL;
int8_t currentRecordIndex = 0; // -1 = prepare once , -2 = prepare loop , >=0 = next position to play/record

unsigned long recordStart = 0;

                              // RPlp : R = record , P = prepare , l = loop , p = play
#define RECORDING_NONE         0b00000000
#define RECORDING_PLAY         0b00010000
#define RECORDING_PREPARE_ONCE 0b01000000
#define RECORDING_PREPARE_LOOP 0b01100000
#define RECORDING_RECORD_ONCE  0b10000000
#define RECORDING_RECORD_LOOP  0b10100000

#define RECORDING_PREPARE_MSK  0b01000000
#define RECORDING_RECORD_MSK   0b10000000
#define RECORDING_LOOP_MSK     0b10100000

byte recordingState = RECORDING_NONE;

void prepareRecord(byte num) {
	if (num >= MAX_RECORDS) {
		return;
	}
	currentRecord = record + num;
	currentRecord->envelop = currentEnvelop;
	currentRecord->harmonic = currentHarmonic;
	currentRecord->loop= (recordingState & RECORDING_LOOP_MSK) ? 1 : 0;
	currentRecordIndex = 0;
	recordingState = RECORDING_RECORD_MSK | (recordingState & RECORDING_LOOP_MSK) | num;
}

void startRecord() {
	recordStart = mozziMicros();
}

void addToRecord(word code) {
	if (!(recordingState & RECORDING_RECORD_MSK) || currentRecordIndex >= MAX_RECORD_ENTRIES) {
		return;
	}
	RecordEntry *entry = &(currentRecord->entries[currentRecordIndex++]);
	entry->key = code;
	entry->timestamp = (mozziMicros() - recordStart) >> 10;
}

inline void stopRecordReplay() {
	recordingState = RECORDING_NONE;
}

void closeRecord() {
	if (!(recordingState & RECORDING_RECORD_MSK)) {
		return;
	}
	addToRecord(0);
	currentRecord->length = currentRecordIndex;
	stopRecordReplay();
}

void startReplay(byte num) {
	if (num >= MAX_RECORDS || record[num].length == 0) {
		return;
	}
	recordStart = mozziMicros();
	currentRecord = record + num;
	currentRecordIndex = 0;
	recordingState = RECORDING_PLAY | num;

	currentHarmonic= currentRecord->harmonic;
	currentEnvelop = currentRecord->envelop;
}

void handleKey(word fullCode);

void replayNext() {
	if (!(recordingState & RECORDING_PLAY)) {
		return;
	}
	RecordEntry *entry = &(currentRecord->entries[currentRecordIndex]);
	if ((mozziMicros() - recordStart) >> 10 < entry->timestamp) {
		return;
	}
	DEBUG( Serial.print("replay "); Serial.print(entry->key);Serial.print(" after "); Serial.print(entry->timestamp); )
	handleKey(entry->key);
	currentRecordIndex++;
	if (currentRecordIndex >= currentRecord->length) {
		if (currentRecord->loop) {
			recordStart = mozziMicros();
			currentRecordIndex = 0;
		} else {
			stopRecordReplay();
		}
	}
}

/* Keyboard constants  Change to suit your Arduino
   define pins used for data and clock from keyboard */
#define DATAPIN 4
#define IRQPIN  3

PS2KeyAdvanced keyboard;

#define FIRST_NOTE 58 // midi code for A#B
//             <=A#=34  Q=B   S=C   X=C#  D=D   C=D#  F=E   G=F   B=F#  H=G   N=G#  J=A   ,=A#  K=B   L=C   :=C#  M=D   !=D#  ù=E   *=F   end
byte keyMap[] = { 0x8B, 0x41, 0x53, 0x58, 0x44, 0x43, 0x46, 0x47, 0x42, 0x48, 0x4E, 0x4A, 0x4D, 0x4B, 0x4C, 0X3D, 0x5B, 0x3E, 0x3A, 0x5C, 0x00 };

word currentKey = 0;

void handleKey(word fullCode) {
	if (fullCode == 0) {
		currentNote = 0;
		currentKey = 0;
		return;
	}

	byte code = fullCode & 0xFF;

	if (fullCode & 0x8000) {
		if (code == PS2_KEY_L_CTRL || code == PS2_KEY_R_CTRL) {
			DEBUG( Serial.print("stop recording"); )
			closeRecord();

		} else if (code == currentKey) {
			addToRecord(0);
			// release currentNote
			currentNote = 0;
			currentKey = 0;
		}

	} else if (code != currentKey) {
		if (code == PS2_KEY_L_CTRL || code == PS2_KEY_R_CTRL) {
			DEBUG( Serial.print("start recording"); )
			startRecord();

		// "drum" keys "=" and backspace
		} else if (code == PS2_KEY_EQUAL) {
			addToRecord(code);
			setSample(100);
		} else if (code == PS2_KEY_BS) {
			addToRecord(code);
			setSample(101);

		// bamboo keys ² , 1-9 , 0
		} else if (code == PS2_KEY_SINGLE) {
			addToRecord(code);
			setSample(0);
		} else if (code >= PS2_KEY_1 && code <= PS2_KEY_9) {
			addToRecord(code);
			setSample(code - PS2_KEY_1 + 1);
		} else if (code == PS2_KEY_0) {
			addToRecord(code);
			setSample(10);

		// signal keys F1..F6
		} else if (code >= PS2_KEY_F1 && code <= PS2_KEY_F6) {
			byte v = code - PS2_KEY_F1;
			if (v < NB_TABLES) {
				currentHarmonic = v;
				DEBUG( Serial.print("mode ");Serial.println(currentHarmonic); )
				updateMode();
			}

		// currentEnvelop keys
		} else if (code >= PS2_KEY_F7 && code <= PS2_KEY_F12) {
			currentEnvelop = code - PS2_KEY_F7;
			DEBUG( Serial.print("envelop ");Serial.println(currentEnvelop); )
			updateEnvelop();

		} else if (code == PS2_KEY_KP_MINUS) {
			recordingState = RECORDING_PREPARE_ONCE;
		} else if (code == PS2_KEY_KP_TIMES) {
			recordingState = RECORDING_PREPARE_LOOP;

		} else if (code >= PS2_KEY_KP0 && code <= PS2_KEY_KP9) {
			byte num = code - PS2_KEY_KP0;
			DEBUG( Serial.print("KP with rec state = "); Serial.println(recordingState, HEX); )
			if (recordingState & RECORDING_PREPARE_MSK) {
				DEBUG( Serial.print("prepare "); Serial.println(num); )
				prepareRecord(num);
			} else if (recordingState == RECORDING_NONE) {
				DEBUG( Serial.print("replay "); Serial.println(num); )
				startReplay(num);
			} else if (recordingState & RECORDING_PLAY) {
				if (num == (recordingState & 0x0F)) {
					DEBUG( Serial.print("stop replay "); Serial.println(num); )
					stopRecordReplay();
				} else {
					DEBUG( Serial.print("replay other "); Serial.println(num); )
					startReplay(num);
				}
			}

		// notes
		} else {
			for (byte *k= keyMap; *k; k++) {
				if (code == *k) {
					addToRecord(code);
					currentKey = code;
					byte note = k-keyMap+FIRST_NOTE;
					DEBUG( Serial.print("note "); Serial.println(note); )
					setNote(note);
					break;
				}
			}
		}
	}
}

void updateControl() {
	switch (currentEnvelop) {
	case 1:
	case 2:
		if (envADSR.playing()) {
			envADSR.update();
		}
		break;
	}

	replayNext();

	if( keyboard.available() ) {
		// read the next key
		word fullCode = keyboard.read()/* & 0x80FF*/; // filter out shift, control, caps, ...
		DEBUG( Serial.print("code ");Serial.println(fullCode, HEX); )

		handleKey(fullCode);
	}
}

AudioOutput_t updateAudio() {
	int32_t base = 0, e = 256, sampleValue = 0;

//	if (currentNote != 0) {
//		for(byte i = 0; i < NB_HARMONICS; i++) {
//			base += harmonicOscils[i].next() * harmonicTables[currentHarmonic].ratios[i];
//		}
//		return MonoOutput::fromNBit(8 + harmonicTables[currentHarmonic].bitShift, base);
//	} else {
//		return 0;
//	}


	if (currentSample == 100 || currentSample == 101) {
		sampleValue = sample.next();
		if (!sample.isPlaying()) {
			currentSample = -1;
			DEBUG( Serial.println("sample end"); )
		}
	} else if (currentSample != -1) {
		sampleValue = bamboo.next();
		if (!bamboo.isPlaying()) {
			currentSample = -1;
			DEBUG( Serial.println("bamboo end"); )
		}
	}

	byte bits = 18; // 8 for value + 8 for envelope + 2 to reduce volume lower to sample one
	if (currentNote != 0) {
//		base = harmonicOscils[0].next();

		switch (currentEnvelop) {
		case 1:
		case 2:
			if (envADSR.playing()) {
				e = envADSR.next();
			} else {
				e = 0;
			}
			break;
		case 3:
		case 4:
			e = envSin.next();
			break;
		}

		for(byte i = 0; i< NB_HARMONICS; i++) {
			base += harmonicOscils[i].next() * harmonicTables[currentHarmonic].ratios[i];
		}
		bits += harmonicTables[currentHarmonic].bitShift;
	}

	return MonoOutput::fromNBit(bits, (base * e) + (sampleValue << (bits - 8)));
}

void setup(){
	DEBUG( Serial.begin(115200); )

	DEBUG( Serial.println("Kbd"); )
	keyboard.begin(DATAPIN, IRQPIN);
	keyboard.setLock(PS2_LOCK_NUM);

	DEBUG( Serial.println("harmonic"); )
	for(byte i = 0; i < NB_HARMONICS; i++) {
		harmonicOscils[i].setTable(SIN1024_DATA);
	}

	DEBUG( Serial.println("bamboo"); )
	bamboo.setFreq((float) BAMBOO_00_1024_SAMPLERATE / (float) BAMBOO_00_1024_NUM_CELLS);
	bamboo.setLoopingOff();
	bamboo.rangeWholeSample();

	DEBUG( Serial.println("bomb"); )
	sample.setFreq((float) ABOMB_SAMPLERATE / (float) ABOMB_NUM_CELLS);
	sample.setLoopingOff();
	sample.setTable(ABOMB_DATA);

	DEBUG( Serial.println("startMozzi"); )
	startMozzi(CONTROL_RATE);

	DEBUG( Serial.println("OK"); )
}

void loop(){
  audioHook(); // required here
}
