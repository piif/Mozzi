import processing.serial.*;
Serial myPort;

void setup() {
  size(1024, 1024);

  println("Available serial ports:");
  println((Object[])Serial.list());

  // myPort = new Serial(this, Serial.list()[0], 9600);  
  myPort = new Serial(this, "/dev/ttyUSB0", 115200);
}

int[] values;

void draw() {
  while (myPort.available() > 0) {
    String inBuffer = myPort.readString();   
    if (inBuffer != null) {
      values = int(inBuffer.split("[\t\n\r]"));
      println(values);
    }

    if (values.length >= 8) {
      clear();
      background(204);
      int timeRange = values[1] + values[3] + values[5] + values[7];
      
      stroke(0);
      int x0 = 0, y0 = 0;
      for (int i = 0; i < 8; i+=2) {
        int x1 = values[i+1] * 1024 / timeRange, y1 = values[i];
        line(x0, 1024-y0, x0+x1, 1024-y1);
        x0 += x1;
        y0 = y1;
      }
    }
  }

  // write the current X-position of the mouse to the serial port as
  // a single byte
  // port.write(mouseX);
}
