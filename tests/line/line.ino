//#define AUDIO_MODE 2 // HIFI
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable

#include <MozziGuts.h>
#include <EventDelay.h>

EventDelay changeDelay;
int value = 80;

void setup(){
	Serial.begin(115200);
	changeDelay.set(1000);

	startMozzi(CONTROL_RATE);
//	TCCR1B = (TCCR1B & ~0b111) | 0b011;
	Serial.println("setupOK");
}

void updateControl(){
	if (changeDelay.ready()) {
		word icr1 = (ICR1H << 8) | ICR1L;
		word ocr1a = (OCR1AH << 8) | OCR1AL;
		word ocr1b = (OCR1BH << 8) | OCR1BL;
		Serial.print("ICR1   = "); Serial.print(ICR1H, HEX); Serial.print(' '); Serial.print(ICR1L, HEX);
		Serial.print(" = "); Serial.println(icr1, HEX);

		Serial.print("OCR1A  = "); Serial.print(OCR1AH, HEX); Serial.print(' '); Serial.print(OCR1AL, HEX);
		Serial.print(" = "); Serial.println(ocr1a, HEX);
		Serial.print("OCR1B  = "); Serial.print(OCR1BH, HEX); Serial.print(' '); Serial.print(OCR1BL, HEX);
		Serial.print(" = "); Serial.println(ocr1b, HEX);

		Serial.print("TCCR1A = "); Serial.println(TCCR1A, HEX);
		Serial.print("TCCR1B = "); Serial.println(TCCR1B, HEX);
		Serial.print("TCCR1C = "); Serial.println(TCCR1C, HEX);
		Serial.print("value  = "); Serial.print(value, HEX); Serial.print("h  = "); Serial.println(value);

//		value <<= 1;
		if (value == 0) {
			value = 10;
		}
		changeDelay.start();
	}
}

AudioOutput_t updateAudio() {
	return value;
}

void loop(){
  audioHook();
}
