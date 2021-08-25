#include <Arduino.h>

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin1024_int8.h>
#include <mozzi_midi.h>

#include <PS2KeyAdvanced.h>

/* Keyboard constants  Change to suit your Arduino
   define pins used for data and clock from keyboard */
#define DATAPIN 4
#define IRQPIN  3

uint16_t c;

PS2KeyAdvanced keyboard;

#define FIRST_NOTE 60 // midi code for C
//              Q=C=36  W=C#  S=D   X=D#  D=E   F=F   V=F#  G=G   B=G#  H=A   N=A#  J=B   K=C   ;=C#  L=D   :=D#  M=E   ù=F   end
byte keyMap[] = { 0x41, 0x5A, 0x53, 0x58, 0x44, 0x46, 0x56, 0x47, 0x42, 0x48, 0x4E, 0x4A, 0x4B, 0x3B, 0x4C, 0x3D, 0x5B, 0x3A, 0x00 };

Oscil <SIN1024_NUM_CELLS, AUDIO_RATE> aSin(SIN1024_DATA);

byte currentNote = 0;

void setNote(byte n) {
	currentNote = n;
	int freq = mtof(currentNote);
	Serial.print("Freq ");
	Serial.println(freq);
	aSin.setFreq(freq);
}

AudioOutput_t updateAudio() {
	if (currentNote) {
		return MonoOutput::from8Bit(aSin.next());
	} else {
		return 0;
	}
}

word currentKey = 0;

void updateControl() {
	if( keyboard.available() ) {
		// read the next key
		word c = keyboard.read();
		if (c > 0) {
			Serial.print( "Value " );
			Serial.print( c, HEX );
			Serial.print( " - Status Bits " );
			Serial.print( c >> 8, HEX );
			Serial.print( "  Code " );
			Serial.println( c & 0xFF, HEX );
			if (c & 0x8000) {
				if (c == (currentKey | 0x8000)) {
					// release currentNote
					currentNote = 0;
					Serial.println('-');
				}
			} else {
				byte code = c & 0xFF;
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

void setup() {
	Serial.begin(115200);

	startMozzi();

	// Configure the keyboard library
	keyboard.begin( DATAPIN, IRQPIN );
	Serial.println( "PS2 Advanced Key Simple Test:" );
}

void loop() {
	audioHook(); // required here
}

