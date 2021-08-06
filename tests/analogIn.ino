#include <Arduino.h>

#define PLUS_1 2   // gray
#define PLUS_2 3   // yellow
#define MINUS_1 4  // green
#define MINUS_2 5  // blue

#define INPUT_1 A0 // black
#define INPUT_2 A1 // red
#define INPUT_3 A2 // brown
#define INPUT_4 A3 // orange

void setup() {
	Serial.begin(115200);
	pinMode(PLUS_1 , INPUT);
	pinMode(PLUS_2 , INPUT);
	pinMode(MINUS_1, INPUT);
	pinMode(MINUS_2, INPUT);
	pinMode(INPUT_1, INPUT);
	pinMode(INPUT_2, INPUT);
	pinMode(INPUT_3, INPUT);
	pinMode(INPUT_4, INPUT);
}

int pots[8] = { 0, };

void loop() {
	pinMode(PLUS_1 , OUTPUT);
	pinMode(MINUS_1, OUTPUT);
	pinMode(PLUS_2 ,  INPUT);
	pinMode(MINUS_2,  INPUT);
	digitalWrite(PLUS_1 , HIGH);
	digitalWrite(MINUS_1, LOW);
	delay(5);
	pots[0] = analogRead(INPUT_1);
	pots[1] = analogRead(INPUT_2);
	pots[2] = analogRead(INPUT_3);
	pots[3] = analogRead(INPUT_4);

	pinMode(PLUS_1 ,  INPUT);
	pinMode(MINUS_1,  INPUT);
	pinMode(PLUS_2 , OUTPUT);
	pinMode(MINUS_2, OUTPUT);
	digitalWrite(PLUS_2 , HIGH);
	digitalWrite(MINUS_2, LOW);
	delay(5);
	pots[4] = analogRead(INPUT_1);
	pots[5] = analogRead(INPUT_2);
	pots[6] = analogRead(INPUT_3);
	pots[7] = analogRead(INPUT_4);

	for(int i=0; i<8; i++) {
		Serial.print(pots[i]); Serial.print('\t');
	}
	Serial.println();

	delay(500);
}
