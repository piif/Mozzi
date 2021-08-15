#include <Arduino.h>
#include "serialInput.h"

#define K_OUT_CLOCK  8  // White
#define K_OUT_CLEAR  9  // blue
#define K_OUT_DATA  10  // gray

#define K_IN_C 5    // orange
#define K_IN_B 6    // yellow
#define K_IN_A 7    // green
#define K_IN_G 3    // brown
#define K_IN_DATA 4 // red

void sendBuffer(byte value) {
	for(byte mask = 0x80; mask; mask >>= 1) {
		digitalWrite(K_OUT_CLOCK, LOW);
//		Serial.println((mask & value) ? HIGH : LOW);
		digitalWrite(K_OUT_DATA, (mask & value) ? HIGH : LOW);
		digitalWrite(K_OUT_CLOCK, HIGH);
	}
	digitalWrite(K_OUT_CLOCK, LOW);
}

byte readInput(byte address) {
	digitalWrite(K_IN_A, (address & 1) ? HIGH : LOW);
	digitalWrite(K_IN_B, (address & 2) ? HIGH : LOW);
	digitalWrite(K_IN_C, (address & 4) ? HIGH : LOW);
	digitalWrite(K_IN_G, LOW);
	byte result = digitalRead(K_IN_DATA);
	digitalWrite(K_IN_G, HIGH);
	return result;
}

char *mapping[8][8] = {
	{	"cx",	"X",	"W",	"sp",	"#",	"9",	"6",	"3"	},
	{	"fc",	"S",	"Q",	"Ct",	"J",	"K",	"L",	"M"	},
	{	"gd",	"Z",	"A",	"sm",	"U",	"I",	"O",	"P"	},
	{	"B",	"C",	"V",	"N",	"0",	"8",	"5",	"2"	},
	{	".",	"es",	",",	"'",	"su",	"rt",	"en",	"rp"	},
	{	"G",	"D",	"F",	"H",	"*",	"7",	"4",	"1"	},
	{	"T",	"E",	"R",	"Y",	";",	"-",	":",	"?"	},
	{	"^",	"cr",	"an",	"v",	"sh",	"<",	">",	"Rt"	}
};

byte scanFirst() {
	for(byte mask = 0x80 , line=7; mask; mask >>= 1, line--) {
		sendBuffer(~mask);
		for(byte adr = 0; adr < 8; adr++) {
			if (readInput(adr) == 0) {
//				Serial.print(line);Serial.print(adr);
				return (line << 4) | adr;
//				return mapping[line][7 - adr];
			}
		}
	}
	return 0xFF;
}

void scanAll() {
	for(byte mask = 0x80; mask; mask >>= 1) {
		sendBuffer(~mask);
		for(byte adr = 0; adr < 8; adr++) {
			Serial.print(readInput(adr));
		}
		Serial.println();
	}
}

void scanAddress(byte address) {
	Serial.println(readInput(address));
}

void scan() {
	for(byte adr = 0; adr < 8; adr++) {
		Serial.print(readInput(adr));
	}
	Serial.println();
}

InputItem inputs[] = {
	{ 'o', 'B', (void *)sendBuffer },
	{ 'i', 'B', (void *)scanAddress },
	{ 's', 'f', (void *)scan },
	{ 'a', 'f', (void *)scanAll }
};

void setup() {
	Serial.begin(115200);

	pinMode(K_OUT_CLOCK, OUTPUT);
	pinMode(K_OUT_CLEAR, OUTPUT);
	pinMode(K_OUT_DATA , OUTPUT);
 	pinMode(K_IN_A     , OUTPUT);
 	pinMode(K_IN_B     , OUTPUT);
 	pinMode(K_IN_C     , OUTPUT);
 	pinMode(K_IN_G     , OUTPUT);
 	pinMode(K_IN_DATA  , INPUT_PULLUP);

 	digitalWrite(K_IN_G, HIGH);
 	digitalWrite(K_OUT_CLOCK, LOW);
 	digitalWrite(K_OUT_CLEAR, HIGH);

 	registerInput(sizeof(inputs), inputs);

 	Serial.print(mapping[2][5]);
 	Serial.print(mapping[0][0]);
 	Serial.print(mapping[7][7]);
 	Serial.println(" => A3^ ?");

 	Serial.println("Setup OK");
}

//char *key = 0;
byte key = 0xFF;
void loop() {
	byte newKey = scanFirst();
	if (newKey != key) {
		key = newKey;
		if (key != 0xFF) {
			Serial.print(key, HEX);
			Serial.print('\t');
			Serial.println(mapping[key >> 4][key & 7]);

		}
	}

//	handleInput();
//	scan();
//	delay(500);
}
