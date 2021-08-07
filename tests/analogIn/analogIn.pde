import processing.serial.*;
Serial myPort;

int[] levels = { 250, 100, 100,  0 };
int[] times  = {  50, 100, 200, 25 };
int[] Xs = new int[4];
int[] Ys = new int[4];

static final char[] levelCommands = { 'a', 'd', 's', 'r' };
static final char[] timeCommands  = { 'A', 'D', 'S', 'R' };

int mouseFromX, mouseFromY, squareFromX, squareFromY;
int currentSquare = -1;
boolean haveToDraw = true;

static final int TOLERANCE = 10;

void mouseMoved() {
	int newSquare = -1;
	for (int i = 0; i < 4; i++) {
		if (abs(mouseX-Xs[i]) < TOLERANCE && abs(mouseY-Ys[i]) < TOLERANCE) {
			mouseFromX = mouseX;
			mouseFromY = mouseY;
			squareFromX = Xs[i];
			squareFromY = Ys[i];
			newSquare = i;
			break;
		}
	}
	if (newSquare != currentSquare) {
		currentSquare = newSquare;
		haveToDraw = true;
	}
}

void mouseDragged() {
	if (currentSquare != -1) {
		// update coords
		int dX = mouseX - mouseFromX;
		int dY = mouseY - mouseFromY;
		if (dX != 0 || dY != 0) {
			Ys[currentSquare] += dY;
			for (int i = currentSquare; i < 4; i++) {
				Xs[i] += dX;
			}
			mouseFromX = mouseX;
			mouseFromY = mouseY;
//			println("drag", currentSquare, Xs[currentSquare], Ys[currentSquare]);
			haveToDraw = true;
		}
	}

//	return true; //result;
}

void buttonReleased() {
	reverseCoords();
}

void prepareCoords() {
	int x0 = 10;
	for (int i = 0; i < 4; i++) {
		Xs[i] = x0 + times[i];
		Ys[i] = 266 - levels[i];
		x0 = Xs[i];
	}
	haveToDraw = true;
}

void reverseCoords() {
	int t0 = 10;
	for (int i = 0; i < 4; i++) {
		times[i] = Xs[i] - t0;
		t0 = Xs[i];
		levels[i] = 266 - Ys[i];
	}
}

void setup() {
	size(1020, 276);

//  println("Available serial ports:");
//  println((Object[])Serial.list());
//  myPort = new Serial(this, Serial.list()[0], 9600);

	myPort = new Serial(this, "/dev/ttyUSB0", 115200);

	prepareCoords();
}

void draw() {
	// TODO : read serial and print result to follow command results
	if (haveToDraw) {
		clear();
		background(204);
		rectMode (CENTER);
		stroke(0);

		int x0 = 10, y0 = 266;
		for (int i = 0; i < 4; i++) {
			line(x0, y0, Xs[i], Ys[i]);
			x0 = Xs[i];
			y0 = Ys[i];
		}
		for (int i = 0; i < 4; i++) {
			fill(i == currentSquare ? 0 : 230);
			square(Xs[i], Ys[i], 6);
		}

		for (int i = 0; i < 4; i++) {
			myPort.write(levelCommands[i]);
			myPort.write(Integer.toString(levels[i]));
			myPort.write('\n');

			myPort.write(timeCommands[i]);
			myPort.write(Integer.toString(times[i]));
			myPort.write('\n');
		}

		haveToDraw = false;
	}
}
