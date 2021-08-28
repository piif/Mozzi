BufferedReader reader;
int X = 0;
int Y = 0;

String line;

void setup() {
	size(2000, 276);
	reader = createReader("/home/pif/GitHub/piif/Mozzi/tests/viewWave/inputPipe");
}

void draw() {
  while(true) {
    try {
      line = reader.readLine();
    } catch (IOException e) {
      e.printStackTrace();
      break;
    }
    if (line != null) {
      if (line.equals("[")) {
        X = 0;
        clear();
        background(204);
      } else if (line.equals("]")) {
        break;
      } else {
        int value = Integer.parseInt(line);
        line(X, 148 - Y, X + 1, 148 - value);
        X++;
        Y = value;
      }
    }
  }
}
