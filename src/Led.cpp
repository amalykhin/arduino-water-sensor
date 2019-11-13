#include <Arduino.h>

class Led {
  int redPin   = 8;
  int greenPin = 9;
  int bluePin  = 10;

  public:
    Led(int redPin, int greenPin, int bluePin) {
      this->redPin = redPin;
      this->greenPin = greenPin;
      this->bluePin = bluePin;
    }

    void light(int red, int green, int blue) {
      digitalWrite(redPin, red);
      digitalWrite(greenPin, green);
      digitalWrite(bluePin, blue);
    }

    void lightRed() {
      light(HIGH, LOW, LOW);
    }

    void lightGreen() {
      light(LOW, HIGH, LOW);
    }

    void lightBlue() {
      light(LOW, LOW, HIGH);
    }
};