#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <EventDelay.h>

#define NB_HARMONICS 5
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> harmonics[NB_HARMONICS];

#define NB_TABLES 5

int ratio[NB_TABLES][NB_HARMONICS] = {
		{  1,  0,  0,  0,  0 },  // simple sinusoïde
		{  8,  4,  0,  2,  0 },  // square
		{ 16,  7,  5,  1,  2 },  // 100, 30, 30, 10, 15 : piano ?
		{  8, 16, 12,  6,  6 },  // 50%, 100, 75, 40, 40: trombone ?
		{ 20, 17, 13,  9,  6 },  // trumpet ?
};
int maxBits[NB_TABLES] {
	1,
	4,
	5,
	6,
	6,
};

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable

int baseFreq = 440;
EventDelay changeDelay;
byte entry = 0;

void setup(){
	Serial.begin(115200);

	for(byte i = 0; i< NB_HARMONICS; i++) {
		harmonics[i].setTable(SIN2048_DATA);
		harmonics[i].setFreq(baseFreq * (i+1));
	}

	changeDelay.set(3000);

	startMozzi(CONTROL_RATE);
}

void updateControl(){
	if (changeDelay.ready()) {
		entry = (entry + 1) % NB_TABLES;
		Serial.println(entry);
		changeDelay.start();
	}
}

AudioOutput_t updateAudio() {
	long value = 0;
	for(byte i = 0; i< NB_HARMONICS; i++) {
		value += harmonics[i].next() * ratio[entry][i];
	}
	return MonoOutput::fromNBit(8+maxBits[entry], value);
}

void loop(){
  audioHook(); // required here
}
