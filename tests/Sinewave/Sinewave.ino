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

#include <MozziGuts.h>
#include <Oscil.h>
#include <EventDelay.h>
#include <tables/sin2048_int8.h> // sine table for oscillator

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> baseSin(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> otherSin(SIN2048_DATA);

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable

EventDelay changeDelay;

int baseFreq = 440;
int otherFreq = baseFreq*4;

void setup(){
	Serial.begin(115200);

	startMozzi(CONTROL_RATE); // :)
	baseSin.setFreq(baseFreq); // set the frequency
	otherSin.setFreq(otherFreq); // set the frequency

	changeDelay.set(3000); // 2 second countdown
}

byte mode = 0;

void updateControl(){
	if (changeDelay.ready()) {
		mode = !mode;
		Serial.println(mode);
		if (mode) {
			baseSin.setFreq(baseFreq); // set the frequency
			otherSin.setFreq(otherFreq); // set the frequency
		} else {
			baseSin.setFreq(baseFreq); // set the frequency
		}
		changeDelay.start();
	}
}

AudioOutput_t updateAudio() {
	if (mode) {
		return MonoOutput::fromNBit(16, baseSin.next() + otherSin.next());
	} else {
		return MonoOutput::from8Bit(baseSin.next()); // return an int signal centred around 0
	}
}

void loop(){
  audioHook(); // required here
}
